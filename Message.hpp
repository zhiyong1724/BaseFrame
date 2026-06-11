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
        Message()
        {
        }

    public:
        int what = 0;
        int arg0 = 0;
        void *arg1 = nullptr;
        IHandler *target = nullptr;
        std::function<void()> handle;
    };
}
#endif
