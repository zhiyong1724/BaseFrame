#ifndef __HANDLER_H__
#define __HANDLER_H__
#include "IHandler.h"
#include "MessageQueue.h"
namespace  BaseFrame
{
    class Handler : public IHandler
    {
    public:
        Handler(MessageQueue *messageQueue);
        virtual ~Handler();
        bool sendMessage(const Message::MessagePtr &message);
        bool post(std::function<void()> handle);
        virtual void handleMessage(void *message) override;
        std::thread::id getDispatchThreadId();

    private:
        MessageQueue *mMessageQueue;
    };
}
#endif
