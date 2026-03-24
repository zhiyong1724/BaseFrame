#ifndef __IHANDLER_H__
#define __IHANDLER_H__
namespace  BaseFrame
{
    class IHandler
    {
    public:
        IHandler() {}
        virtual ~IHandler() {}
        virtual void handleMessage(void *message) = 0;
    };
}
#endif
