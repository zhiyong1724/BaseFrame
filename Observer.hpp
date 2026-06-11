#ifndef __OBSERVER_HPP__
#define __OBSERVER_HPP__
#include "Any.hpp"
#include <functional>
namespace  BaseFrame
{
    template <typename T0>
    class Observer : public Any
    {
    public:
        template <typename T1>
        Observer(const T1 &t) : Any(), notify(t) {}
        virtual ~Observer() {}

    public:
        std::function<T0> notify;
    };
}
#endif
