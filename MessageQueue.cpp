#include "MessageQueue.h"
namespace  BaseFrame
{
    MessageQueue::MessageQueue() : mMaxMsgCount(100)
    {
    }

    MessageQueue::~MessageQueue()
    {
    }

    MessageQueue &MessageQueue::getInstance()
    {
        static MessageQueue sMessageQueue;
        return sMessageQueue;
    }

    bool MessageQueue::pushMessage(const Message::MessagePtr &message)
    {
        std::lock_guard<std::mutex> autolock(mMutex);
        if (mMessages.size() < mMaxMsgCount)
        {
            mMessages.push_back(message);
            return true;
        }
        return false;
    }

    void MessageQueue::dispatchMessage()
    {
        mMutex.lock();
        Messages messages = mMessages;
        mMessages.clear();
        mMutex.unlock();
        for (auto &message : messages)
        {
            if (message->target != nullptr)
            {
                message->target->handleMessage(message.get());
            }
            else
            {
                message->handle();
            }
        }
    }

    void MessageQueue::clearMessage()
    {
        std::lock_guard<std::mutex> autolock(mMutex);
        mMessages.clear();
    }

    void MessageQueue::setMaxMsgCount(int n)
    {
        std::lock_guard<std::mutex> autolock(mMutex);
        mMaxMsgCount = n;
    }
}