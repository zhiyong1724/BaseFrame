#ifndef __VIEW_DATA_H__
#define __VIEW_DATA_H__
#include "Handler.h"
#include "MessageQueue.h"
#include <memory>
#include <list>
#include "LiveState.h"
#include <functional>
#include <exception>
#include <atomic>
#include <shared_mutex>
#include <mutex>
namespace BaseFrame
{
    template <typename T>
    class ViewData
    {
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
            mVersion.fetch_add(1, std::memory_order_acq_rel);
            {
                std::lock_guard<std::shared_mutex> lock(mValueLock);
                mValue = t;
            }
            notify();
        }

        void postValue(const T &t)
        {
            mVersion.fetch_add(1, std::memory_order_acq_rel);
            unsigned int version = mVersion.load(std::memory_order_acquire);
            mHandler.post([this, t, version]() { //
                if (version != mVersion.load(std::memory_order_acquire))
                {
                    return;
                }
                {
                    std::lock_guard<std::shared_mutex> lock(mValueLock);
                    mValue = t;
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
        void observe(std::weak_ptr<LiveState> liveState, std::function<void(const T &)> observer)
        {
            if (std::this_thread::get_id() != mHandler.getDispatchThreadId())
            {
                throw std::runtime_error("not in main thread");
            }
            mObservers.push_back(std::make_pair(liveState, observer));
            notifyAll();
        }

        ViewData &operator=(const ViewData &t) = delete;
    private:
        void notify()
        {
            auto value = getValue();
            for (auto itr = mObservers.begin(); itr != mObservers.end(); )
            {
                auto liveState = itr->first.lock();
                if (liveState != nullptr)
                {
                    switch (liveState->getState())
                    {
                    case LiveState::STATE_STARTED:
                    case LiveState::STATE_RESUMED:
                    case LiveState::STATE_PAUSED:
                        itr->second(value);
                        break;
                    default:
                        break;
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
                    itr->second(value);
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
        std::list<std::pair<std::weak_ptr<LiveState>, std::function<void(const T &)>>> mObservers;
    };
}
#endif
