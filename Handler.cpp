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
        return mMessageQueue->pushMessage(message);
    }

    bool Handler::sendMessage(const Message::MessagePtr &message)
    {
        message->target = this;
        return mMessageQueue->pushMessage(message);
    }

    void Handler::handleMessage(void *message)
    {
    }
}
