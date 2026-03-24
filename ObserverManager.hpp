#ifndef __OBSERVER_MANAGER_HPP__
#define __OBSERVER_MANAGER_HPP__
#include "Observer.hpp"
#include "Subject.hpp"
#include <memory>
#include <map>
#include <string>
#include <mutex>
namespace  BaseFrame
{
    class ObserverManager
    {
    public:
        ~ObserverManager() {}

        static ObserverManager &getInstance()
        {
            static ObserverManager sObserverManager;
            return sObserverManager;
        }

        template <typename T>
        void registerObserver(const std::string &observerName, const T &observer)
        {
            std::lock_guard<std::mutex> autolock(mLock);
            mObservers.insert(std::make_pair(observerName, std::make_shared<T>(observer)));
        }

        void unregisterObserver(const std::string &observerName)
        {
            std::lock_guard<std::mutex> autolock(mLock);
            auto itr = mObservers.find(observerName);
            if (itr != mObservers.end())
            {
                mObservers.erase(itr);
            }
        }

        template <typename T>
        std::shared_ptr<Subject<T>> getSubject(const std::string &observerName)
        {
            std::lock_guard<std::mutex> autolock(mLock);
            auto itr = mObservers.find(observerName);
            if (itr != mObservers.end())
            {
                auto observer = (Subject<T> *)itr->second.get();
                return std::make_shared<Subject<T>>(*observer);
            }
            return nullptr;
        }

    private:
        ObserverManager() {}

    private:
        std::map<std::string, std::shared_ptr<Any>> mObservers;
        std::mutex mLock;
    };
}
#endif