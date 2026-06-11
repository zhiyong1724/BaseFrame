#ifndef __LIVE_STATE_H__
#define __LIVE_STATE_H__
namespace BaseFrame
{
    class LiveState
    {
    public:
        enum State
        {
            STATE_UNINIT,
            STATE_CREATED,
            STATE_STARTED,
            STATE_RESUMED,
            STATE_PAUSED,
            STATE_STOPED,
            STATE_DESTROYED
        };

    public:
        LiveState() {}
        virtual ~LiveState() {}
        virtual void setState(State state) = 0;
        virtual State getState() = 0;
    };
}
#endif