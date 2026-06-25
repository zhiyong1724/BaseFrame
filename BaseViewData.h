#ifndef __BASE_VIEW_DATA__
#define __BASE_VIEW_DATA__
#include "Any.h"
#include "LiveState.h"
#include <memory>
namespace BaseFrame
{
    class BaseViewData : public Any
    {
    public:
        BaseViewData() : Any() {}
        virtual ~BaseViewData() {}
        virtual void notify(const std::weak_ptr<LiveState> &target = std::weak_ptr<LiveState>()) = 0;
    };
}
#endif