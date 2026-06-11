#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__
#include <vector>
#include <mutex>
#include "Message.hpp"
#include <thread>
#include <atomic>
namespace BaseFrame
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
        std::thread::id getDispatchThreadId();

    private:
        Messages mMessages;
        std::mutex mMutex;
        size_t mMaxMsgCount = 4096;
        std::atomic<std::thread::id> mDispatchThreadId;
    };
}
#endif
