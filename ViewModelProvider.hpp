#ifndef __VIEW_MODEL_PROVIDER_H__
#define __VIEW_MODEL_PROVIDER_H__
#include "ViewModel.h"
#include <memory>
#include <map>
#include <typeindex>
namespace BaseFrame
{
    class ViewModelProvider
    {
    public:
        ~ViewModelProvider() {}
        static ViewModelProvider &getInstance()
        {
            static ViewModelProvider sViewModelProvider;
            return sViewModelProvider;
        }
        template <typename T>
        std::shared_ptr<T> getViewModel()
        {
            std::shared_ptr<T> viewModel = nullptr;
            auto itr = mViewModelMap.find(std::type_index(typeid(T)));
            if (itr != mViewModelMap.end())
            {
                viewModel = std::dynamic_pointer_cast<T>(itr->second.lock());
            }
            if (viewModel == nullptr)
            {
                viewModel = std::make_shared<T>(ViewModel::Token());
                mViewModelMap[std::type_index(typeid(T))] = viewModel;
            }
            return viewModel;
        }

    private:
        ViewModelProvider() {}

    private:
        std::map<std::type_index, std::weak_ptr<ViewModel>> mViewModelMap;
    };
}
#endif