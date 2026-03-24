#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__
#include <vector>
#include <mutex>
#include "Message.hpp"
namespace  BaseFrame
{
    class MessageQueue
    {
    public:
        typedef std::vector<Message::MessagePtr> Messages;
    public:
        MessageQueue();
        ~MessageQueue();
        static MessageQueue &getInstance();
        bool pushMessage(const Message::MessagePtr &message);
        void dispatchMessage();
        void clearMessage();
        void setMaxMsgCount(int n);

    private:
        Messages mMessages;
        std::mutex mMutex;
        size_t mMaxMsgCount;
    };
}
#endif
