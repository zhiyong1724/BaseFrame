#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__
#include <string>
#include "Message.hpp"
#include "LanguageManager.h"
#include "LiveState.h"
#include <memory>
#include <set>
namespace BaseFrame
{
    class ActivityManager;
    class Activity : public std::enable_shared_from_this<Activity>, public LiveState
    {
    public:
        friend ActivityManager;
        enum ActivityType
        {
            ACTIVITY_TYPE_NORMAL,
            ACTIVITY_TYPE_FRAGMENT
        };

        struct Configuration
        {
            LanguageManager::Language language;
            Configuration() : language(LanguageManager::Language::LANGUAGE_EN) {}
            bool operator!=(const Configuration &configuration) const
            {
                if (language != configuration.language)
                {
                    return true;
                }
                return false;
            }

            void operator=(const Configuration &configuration)
            {
                language = configuration.language;
            }
        };

    public:
        Activity(ActivityType type = ACTIVITY_TYPE_NORMAL);
        virtual ~Activity();
        void startActivity(const std::string &name, const BaseFrame::Message::MessagePtr &message);
        void finish();

    protected:
        virtual void onCreate();
        virtual void onStart();
        virtual void onResume(const BaseFrame::Message::MessagePtr &message);
        virtual void onPause();
        virtual void onStop();
        virtual void onDestroy();
        virtual void onConfigurationChanged(const Configuration &configuration);

    private:
        virtual void setState(State state) override;
        virtual State getState() override;
        virtual void addViewDataReference(const Any *viewData) override;

    private:
        State mState = STATE_UNINIT;
        ActivityType mActivityType;
        Configuration mConfiguration;
        std::set<Any *> mViewDataReferences;
    };
}
#endif