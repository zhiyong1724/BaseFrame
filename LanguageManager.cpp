#include "LanguageManager.h"
#include "ObserverManager.hpp"
namespace BaseFrame
{
    LanguageManager::LanguageManager()
    {
    }

    LanguageManager::~LanguageManager() 
    {
    }

    LanguageManager &LanguageManager::getInstance() 
    {
        static LanguageManager sLanguageManager;
        return sLanguageManager;
    }

    void LanguageManager::addLanguage(Language language, std::shared_ptr<Strings> Strings)
    {
        mLanguageMap.insert(std::make_pair(language, Strings));
    }

    void LanguageManager::removeLanguage(Language language)
    {
        auto itr = mLanguageMap.find(language);
        if (itr != mLanguageMap.end())
        {
            mLanguageMap.erase(itr);
        }
    }

    void LanguageManager::setLanguage(Language language) 
    {
        mLanguage = language;
        auto &observerManager = ObserverManager::getInstance();
        auto observer = observerManager.getObserver<void(Language)>("ActivityManager::setLanguage");
        if (observer)
            observer->notify(mLanguage);
    }

    LanguageManager::Language LanguageManager::getLanguage() 
    {
        return mLanguage;
    }

    std::string LanguageManager::getString(const std::string &key) 
    {
        auto itr = mLanguageMap.find(mLanguage);
        if (itr != mLanguageMap.end())
        {
            auto strings = itr->second;
            return strings->getString(key);
        }
        return "";
    }
}