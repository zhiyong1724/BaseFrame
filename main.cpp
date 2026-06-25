#include <stdio.h>
#include "ObserverManager.hpp"
#include "Activity.h"
#include "ViewModel.h"
#include "ViewData.hpp"
#include "ViewModelProvider.hpp"
#include "ActivityManager.h"
#include "MessageQueue.h"
class MainViewModel : public BaseFrame::ViewModel
{
public:
    MainViewModel(Token token) : ViewModel(token) {}
    virtual ~MainViewModel() {}

public:
    BaseFrame::ViewData<int> mValue;
};

class MainActivity : public BaseFrame::Activity
{
public:
    MainActivity() : Activity() {}
    virtual ~MainActivity() {}

protected:
    virtual void onCreate() override
    {
        printf("onCreate\n");
        mMainViewModel = BaseFrame::ViewModelProvider::getInstance().getViewModel<MainViewModel>();
        mMainViewModel->mValue.observe(shared_from_this(), [](const int &value) { //
            printf("value:%d\n", value);
        });
    }

    virtual void onStart() override
    {
        printf("onStart\n");
    }

    virtual void onResume(const BaseFrame::Message::MessagePtr &message) override
    {
        printf("onResume\n");
        mMainViewModel->mValue.postValue(10);
    }

    virtual void onPause() override
    {
        printf("onPause\n");
    }

    virtual void onStop() override
    {
        printf("onStop\n");
    }

    virtual void onDestroy() override
    {
        printf("onDestroy\n");
    }

    virtual void onConfigurationChanged(const Configuration &configuration) override
    {
        printf("onConfigurationChanged\n");
    }

private:
    std::shared_ptr<MainViewModel> mMainViewModel;
};

int main()
{
    auto &observerManager = BaseFrame::ObserverManager::getInstance();
    observerManager.registerObserver("hello", BaseFrame::Observer<void(int)>([](int i) { //
                                         printf("Hello%d\n", i);
                                     }));

    auto observer = observerManager.getObserver<void(int)>("hello");
    observer->notify(20);

    auto &activityManager = BaseFrame::ActivityManager::getInstance();
    activityManager.registerActivity<MainActivity>("MainActivity");
    activityManager.startActivity("MainActivity", nullptr);

    auto &messageQueue = BaseFrame::MessageQueue::getInstance();
    for (;;)
    {
        messageQueue.dispatchMessage();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}