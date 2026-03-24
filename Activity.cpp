#include "Activity.h"
#include "ActivityManager.h"
namespace BaseFrame
{
    Activity::Activity() : mIsStarted(false)
    {
    }

    Activity::~Activity()
    {
    }

    bool Activity::startActivity(const std::string &name, const BaseFrame::Message::MessagePtr &message)
    {
        auto &activityManager = ActivityManager::getInstance();
        return activityManager.startActivity(name, message);
    }

    void Activity::exit()
    {
        auto &activityManager = ActivityManager::getInstance();
        activityManager.exit();
    }

    void Activity::onCreate()
    {
    }

    void Activity::onStart(const BaseFrame::Message::MessagePtr &message)
    {
    }

    void Activity::onRestart(const BaseFrame::Message::MessagePtr &message)
    {
    }

    void Activity::onStop()
    {
    }

    void Activity::onDestroy()
    {
    }
}