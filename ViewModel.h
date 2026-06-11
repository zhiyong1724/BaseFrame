#ifndef __VIEW_MODEL_H__
#define __VIEW_MODEL_H__
namespace BaseFrame
{
    class ViewModelProvider;
    class ViewModel
    {
    public:
        class Token
        {
            friend ViewModelProvider;

        public:
            ~Token() {}

        private:
            Token() {}
        };

    public:
        ViewModel() = delete;
        virtual ~ViewModel() {}

    protected:
        explicit ViewModel(Token token) {}
    };
}
#endif