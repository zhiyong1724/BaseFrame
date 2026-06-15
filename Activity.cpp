#include "Activity.h"
#include "ActivityManager.h"
namespace BaseFrame
{
    Activity::Activity(ActivityType type) : mActivityType(type)
    {
    }

    Activity::~Activity()
    {
    }

    void Activity::startActivity(const std::string &name, const BaseFrame::Message::MessagePtr &message)
    {
        auto &activityManager = ActivityManager::getInstance();
        activityManager.startActivity(name, message);
    }

    void Activity::finish()
    {
        auto &activityManager = ActivityManager::getInstance();
        activityManager.finish(shared_from_this());
    }

    void Activity::onCreate()
    {
    }

    void Activity::onStart()
    {
    }

    void Activity::onResume(const BaseFrame::Message::MessagePtr &message)
    {
    }

    void Activity::onPause()
    {
    }

    void Activity::onStop()
    {
    }

    void Activity::onDestroy()
    {
    }

    void Activity::onConfigurationChanged(const Configuration &configuration)
    {
    }

    void Activity::setState(State state)
    {
        mState = state;
    }

    Activity::State Activity::getState()
    {
        return mState;
    }
}