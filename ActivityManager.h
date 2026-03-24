#ifndef __ACTIVITY_MANAGER_H__
#define __ACTIVITY_MANAGER_H__
#include "Activity.h"
#include <list>
#include <memory>
#include <map>
#include <string>
#include <mutex>
#include "Handler.h"
namespace BaseFrame
{
    class ActivityManager
    {
    public:
        ~ActivityManager();
        static ActivityManager &getInstance();
        void registerActivity(const std::string &name, const std::shared_ptr<Activity> &activity);
        void unregisterActivity(const std::string &name); 
        bool startActivity(const std::string &name, const Message::MessagePtr &message);
        void exit(); 
    private:
        ActivityManager();
        void removeFromTrack(const std::shared_ptr<Activity> &activity);

    private:
        std::mutex mLock;
        std::map<std::string, std::shared_ptr<Activity>> mActivityMap;
        std::list<std::shared_ptr<Activity>> mActivityTrack;
        Handler mHandler;
    };
}
#endif