#ifndef __OBSERBER_HPP__
#define __OBSERBER_HPP__
#include "Any.hpp"
#include <functional>
namespace  BaseFrame
{
    template <typename T0>
    class Observer : public Any
    {
    public:
        template <typename T1>
        Observer(const T1 &t) : Any(), func(t) {}
        virtual ~Observer() {}

    public:
        std::function<T0> func;
    };
}
#endif