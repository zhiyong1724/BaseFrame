#include "Handler.h"
namespace  BaseFrame
{
    Handler::Handler(MessageQueue *messageQueue) : mMessageQueue(messageQueue)
    {
    }

    Handler::~Handler()
    {
    }

    bool Handler::post(std::function<void()> handle)
    {
        auto message = Message::obtain();
        message->handle = handle;
        if (mMessageQueue)
            return mMessageQueue->pushMessage(message);
        return false;
    }

    bool Handler::sendMessage(const Message::MessagePtr &message)
    {
        message->target = this;
        if (mMessageQueue)
            return mMessageQueue->pushMessage(message);
        return false;
    }

    void Handler::handleMessage(void *message)
    {
    }

    std::thread::id Handler::getDispatchThreadId()
    {
        return mMessageQueue->getDispatchThreadId();
    }
}
