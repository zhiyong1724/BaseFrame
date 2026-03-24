#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__
#include <string>
#include "Message.hpp"
namespace BaseFrame
{
    class ActivityManager;
    class Activity
    {
    public:
        friend ActivityManager;

    public:
        Activity();
        virtual ~Activity();
        bool startActivity(const std::string &name, const BaseFrame::Message::MessagePtr &message);
        void exit();
    private:
        virtual void onCreate();
        virtual void onStart(const BaseFrame::Message::MessagePtr &message);
        virtual void onRestart(const BaseFrame::Message::MessagePtr &message);
        virtual void onStop();
        virtual void onDestroy();

    private:
        bool mIsStarted;
    };
}
#endif