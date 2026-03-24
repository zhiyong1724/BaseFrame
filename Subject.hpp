#ifndef __SUBJECT_HPP__
#define __SUBJECT_HPP__
#include <functional>
namespace  BaseFrame
{
    template <typename T>
    class Subject
    {
    public:
        Subject() {}
        virtual ~Subject() {}

    public:
        std::function<T> call;
    };
}
#endif