#ifndef __ACTIVITY_MANAGER_H__
#define __ACTIVITY_MANAGER_H__
#include "Activity.h"
#include <list>
#include <map>
#include <string>
#include "Handler.h"
namespace BaseFrame
{
    class ActivityManager
    {
    public:
        ~ActivityManager();
        static ActivityManager &getInstance();
        template<typename T>
        void registerActivity(const std::string &name) 
        {
            mActivityFactoryMap.insert(std::make_pair(name, []() -> std::shared_ptr<Activity> { return std::make_shared<T>(); }));
        }
        void unregisterActivity(const std::string &name);
        void startActivity(const std::string &name, const Message::MessagePtr &message);
        void finish(const std::shared_ptr<Activity> &activity); 
    private:
        ActivityManager();
        void removeFromTrack(const std::shared_ptr<Activity> &activity);
        void removeFromActivityMap(const std::shared_ptr<Activity> &activity);
        void setLanguage(LanguageManager::Language language);
        void checkConfigurationChanged(const std::shared_ptr<Activity> &activity);
    private:
        std::map<std::string, std::shared_ptr<Activity>> mActivityMap;
        std::map<std::string, std::function<std::shared_ptr<Activity>()>> mActivityFactoryMap;
        std::list<std::shared_ptr<Activity>> mActivityTrack;
        Handler mHandler;
        Activity::Configuration mConfiguration;
    };
}
#endif