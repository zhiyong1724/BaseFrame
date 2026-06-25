#ifndef __VIEW_DATA_H__
#define __VIEW_DATA_H__
#include "Handler.h"
#include "MessageQueue.h"
#include <list>
#include <functional>
#include <exception>
#include <atomic>
#include <shared_mutex>
#include <mutex>
#include "BaseViewData.h"
namespace BaseFrame
{
    template <typename T>
    class ViewData : public BaseViewData
    {
    private:
        struct ObserverWrapper
        {
            std::function<void(const T &)> mObserver;
            unsigned int mVersion = 0;
            ObserverWrapper(const std::function<void(const T &)> &observer) : mObserver(observer) {}
        };

    public:
        ViewData() : mHandler(&MessageQueue::getInstance())
        {
        }
        ViewData(const T &t) : mValue(t), mHandler(&MessageQueue::getInstance())
        {
        }
        virtual ~ViewData() {}
        //只能在主线程调用
        void setValue(const T &t)
        {
            if (std::this_thread::get_id() != mHandler.getDispatchThreadId())
            {
                throw std::runtime_error("not in main thread");
            }
            {
                std::lock_guard<std::shared_mutex> lock(mValueLock);
                mValue = t;
                mVersion.fetch_add(1, std::memory_order_acq_rel);
            }
            notify();
        }

        void postValue(const T &t)
        {
            unsigned int version = 0;
            {
                std::lock_guard<std::shared_mutex> lock(mValueLock);
                mValue = t;
                mVersion.fetch_add(1, std::memory_order_acq_rel);
                version = mVersion.load(std::memory_order_acquire);
            }
            mHandler.post([this, version]() { //
                if (version != mVersion.load(std::memory_order_acquire))
                {
                    return;
                }
                notify();
            });
        }

        T getValue()
        {
            std::shared_lock<std::shared_mutex> lock(mValueLock);
            auto value = mValue;
            return value;
        }
        //只能在主线程调用
        void observe(const std::shared_ptr<LiveState> &liveState, const std::function<void(const T &)> &observer)
        {
            if (std::this_thread::get_id() != mHandler.getDispatchThreadId())
            {
                throw std::runtime_error("not in main thread");
            }
            mObservers.push_back(std::make_pair(liveState, observer));
            liveState->addViewDataReference(this);
            notifyAll();
        }

        ViewData &operator=(const ViewData &t) = delete;
    private:
        virtual void notify(const std::weak_ptr<LiveState> &target = std::weak_ptr<LiveState>()) override
        {
            auto value = getValue();
            for (auto itr = mObservers.begin(); itr != mObservers.end(); )
            {
                auto liveState = itr->first.lock();
                if (liveState != nullptr)
                {
                    if (target.expired() || (liveState == target.lock() && itr->second.mVersion != mVersion.load(std::memory_order_acquire)))   
                    {
                        switch (liveState->getState())
                        {
                        case LiveState::STATE_STARTED:
                        case LiveState::STATE_RESUMED:
                        case LiveState::STATE_PAUSED:
                            itr->second.mVersion = mVersion.load(std::memory_order_acquire);
                            itr->second.mObserver(value);
                            break;
                        default:
                            break;
                        }
                    }
                    itr++;
                }
                else
                {
                    itr = mObservers.erase(itr);
                }
            }
        }

        void notifyAll()
        {
            auto value = getValue();
            for (auto itr = mObservers.begin(); itr != mObservers.end(); )
            {
                auto liveState = itr->first.lock();
                if (liveState != nullptr)
                {
                    itr->second.mVersion = mVersion.load(std::memory_order_acquire);
                    itr->second.mObserver(value);
                    itr++;
                }
                else
                {
                    itr = mObservers.erase(itr);
                }
            }
        }

    private:
        T mValue;
        std::shared_mutex mValueLock;
        std::atomic<unsigned int> mVersion = 0;
        Handler mHandler;
        std::list<std::pair<std::weak_ptr<LiveState>, ObserverWrapper>> mObservers;
    };
}
#endif
