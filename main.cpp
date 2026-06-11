#include <stdio.h>
#include "ObserverManager.hpp"
int main() 
{
    auto &observerManager = BaseFrame::ObserverManager::getInstance();
    observerManager.registerObserver("hello", BaseFrame::Observer<void(int)>([](int i){
        printf("Hello%d\n", i);
    }));

    auto observer = observerManager.getObserver<void(int)>("hello");
    observer->notify(20);
    return 0;
}