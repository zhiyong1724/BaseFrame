#include "ActivityManager.h"
#include "ObserverManager.hpp"
namespace BaseFrame
{
    ActivityManager::ActivityManager() : mHandler(&MessageQueue::getInstance())
    {
        auto language = LanguageManager::getInstance().getLanguage();
        mConfiguration.language = language;

        auto &observerManager = ObserverManager::getInstance();
        observerManager.registerObserver("ActivityManager::setLanguage", Observer<void(LanguageManager::Language)>(std::bind(&ActivityManager::setLanguage, this, std::placeholders::_1)));
    }

    ActivityManager::~ActivityManager()
    {
    }

    ActivityManager &ActivityManager::getInstance()
    {
        static ActivityManager sActivityManager;
        return sActivityManager;
    }

    void ActivityManager::unregisterActivity(const std::string &name)
    {
        auto itr = mActivityFactoryMap.find(name);
        if (itr != mActivityFactoryMap.end())
        {
            mActivityFactoryMap.erase(itr);
        }
    }

    void ActivityManager::startActivity(const std::string &name, const Message::MessagePtr &message)
    {
        mHandler.post([this, name, message]() { //
            auto factoryItr = mActivityFactoryMap.find(name);
            if (factoryItr != mActivityFactoryMap.end())
            {
                std::shared_ptr<Activity> activity = nullptr;
                auto activityItr = mActivityMap.find(name);
                if (activityItr != mActivityMap.end())
                {
                    activity = activityItr->second;
                    removeFromTrack(activity);
                }
                else
                {
                    auto factory = factoryItr->second;
                    activity = factory();
                    activity->mConfiguration = mConfiguration;
                    activity->mState = Activity::STATE_CREATED;
                    mActivityMap.insert(std::make_pair(name, activity));
                }

                if (!mActivityTrack.empty())
                {
                    auto &stopActivity = mActivityTrack.back();
                    stopActivity->mState = Activity::STATE_PAUSED;
                    stopActivity->onPause();
                    if (Activity::ACTIVITY_TYPE_NORMAL == activity->mActivityType)
                    {
                        stopActivity->mState = Activity::STATE_STOPED;
                        stopActivity->onStop();
                    }
                }
                mActivityTrack.push_back(activity);
                switch (activity->mState)
                {
                case Activity::STATE_CREATED:
                    activity->onCreate();
                    activity->mState = Activity::STATE_STARTED;
                    activity->onStart();
                    activity->mState = Activity::STATE_RESUMED;
                    activity->onResume(message);
                    break;
                case Activity::STATE_STOPED:
                    activity->mState = Activity::STATE_STARTED;
                    activity->onStart();
                    activity->mState = Activity::STATE_RESUMED;
                    activity->onResume(message);
                    checkConfigurationChanged(activity);
                    break;
                case Activity::STATE_PAUSED:
                    activity->mState = Activity::STATE_RESUMED;
                    activity->onResume(message);
                    break;
                default:
                    break;
                }
            }
        });
    }

    void ActivityManager::finish(const std::shared_ptr<Activity> &activity)
    {
        mHandler.post([this, activity]() { //
            if (activity->mState == Activity::STATE_RESUMED)
            {
                activity->mState = Activity::STATE_PAUSED;
                activity->onPause();
            }
            if (activity->mState == Activity::STATE_STARTED || activity->mState == Activity::STATE_PAUSED)
            {
                activity->mState = Activity::STATE_STOPED;
                activity->onStop();
            }
            if (activity->mState == Activity::STATE_CREATED || activity->mState == Activity::STATE_STOPED)
            {
                activity->mState = Activity::STATE_DESTROYED;
                activity->onDestroy();
                removeFromTrack(activity);
                removeFromActivityMap(activity);
            }
            if (!mActivityTrack.empty())
            {
                auto &activity = mActivityTrack.back();
                switch (activity->mState)
                {
                case Activity::STATE_STOPED:
                    activity->mState = Activity::STATE_STARTED;
                    activity->onStart();
                    activity->mState = Activity::STATE_RESUMED;
                    activity->onResume(nullptr);
                    checkConfigurationChanged(activity);
                    break;
                case Activity::STATE_PAUSED:
                    activity->mState = Activity::STATE_RESUMED;
                    activity->onResume(nullptr);
                    break;
                default:
                    break;
                }
            }
        });
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

    void ActivityManager::removeFromActivityMap(const std::shared_ptr<Activity> &activity)
    {
        for (auto itr = mActivityMap.begin(); itr != mActivityMap.end(); itr++)
        {
            if (itr->second == activity)
            {
                mActivityMap.erase(itr);
                break;
            }
        }
    }

    void ActivityManager::setLanguage(LanguageManager::Language language)
    {
        mHandler.post([this, language]() { //
            mConfiguration.language = language;
            for (auto &activity : mActivityTrack)
            {
                switch (activity->mState)
                {
                case Activity::STATE_STARTED:
                case Activity::STATE_RESUMED:
                case Activity::STATE_PAUSED:
                    checkConfigurationChanged(activity);
                    break;
                default:
                    break;
                }
            }
        });
    }

    void ActivityManager::checkConfigurationChanged(const std::shared_ptr<Activity> &activity)
    {
        if (activity->mConfiguration != mConfiguration)
        {
            activity->mConfiguration = mConfiguration;
            activity->onConfigurationChanged(mConfiguration);
        }
    }
}