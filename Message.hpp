#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__
#include <memory>
#include "IHandler.h"
#include <functional>
namespace  BaseFrame
{
    class Message
    {
    public:
        typedef std::shared_ptr<Message> MessagePtr;

    public:
        virtual ~Message()
        {
        }
        static MessagePtr obtain()
        {
            MessagePtr ptr(new Message());
            return ptr;
        }

    private:
        Message() : target(nullptr)
        {
            what = 0;
            arg0 = 0;
            arg1 = nullptr;
        }

    public:
        int what;
        int arg0;
        void *arg1;
        IHandler *target;
        std::function<void()> handle;
    };
}
#endif
