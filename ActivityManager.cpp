#include "ActivityManager.h"
namespace BaseFrame
{
    ActivityManager::ActivityManager() : mHandler(&MessageQueue::getInstance())
    {
    }

    ActivityManager::~ActivityManager()
    {
    }

    ActivityManager &ActivityManager::getInstance()
    {
        static ActivityManager sActivityManager;
        return sActivityManager;
    }

    void ActivityManager::registerActivity(const std::string &name, const std::shared_ptr<Activity> &activity)
    {
        std::lock_guard<std::mutex> autolock(mLock);
        activity->onCreate();
        mActivityMap.insert(std::make_pair(name, activity));
    }

    void ActivityManager::unregisterActivity(const std::string &name)
    {
        std::lock_guard<std::mutex> autolock(mLock);
        auto itr = mActivityMap.find(name);
        if (itr != mActivityMap.end())
        {
            auto activity = itr->second;
            removeFromTrack(itr->second);
            mActivityMap.erase(itr);
            activity->onDestroy();
        }
    }

    bool ActivityManager::startActivity(const std::string &name, const Message::MessagePtr &message)
    {
        std::lock_guard<std::mutex> autolock(mLock);
        auto itr = mActivityMap.find(name);
        if (itr != mActivityMap.end())
        {
            if (!mActivityTrack.empty())
            {
                auto &stopActivity = mActivityTrack.back();
                mHandler.post([stopActivity](){
                    stopActivity->onStop();
                });
            }
            auto &activity = itr->second;
            removeFromTrack(activity);
            mActivityTrack.push_back(activity);
            if (activity->mIsStarted)
            {
                mHandler.post([activity, message](){
                    activity->onRestart(message);
                });
            }
            else
            {
                mHandler.post([activity, message](){
                    activity->onStart(message);
                });
            }
            return true;
        }
        return false;
    }

    void ActivityManager::exit()
    {
        std::lock_guard<std::mutex> autolock(mLock);
        if (!mActivityTrack.empty())
        {
            auto &stopActivity = mActivityTrack.back();
            mHandler.post([stopActivity](){
                    stopActivity->onStop();
                });
            mActivityTrack.pop_back();

            if (!mActivityTrack.empty())
            {
                auto &activity = mActivityTrack.back();
                mHandler.post([activity](){
                    activity->onRestart(nullptr);
                });
            }
        }
    }

    void ActivityManager::removeFromTrack(const std::shared_ptr<Activity> &activity)
    {
        for (auto itr = mActivityTrack.begin(); itr != mActivityTrack.end(); itr++)
        {
            if (*itr == activity)
            {
                mActivityTrack.erase(itr);
                break;
            }
        }
    }
}