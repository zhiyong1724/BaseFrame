#ifndef __LANGUAGE_MANAGER_H__
#define __LANGUAGE_MANAGER_H__
#include "Strings.h"
#include <map>
#include <memory>
namespace BaseFrame
{
    class LanguageManager
    {
    public:
        enum Language
        {
            LANGUAGE_EN,    // 英语 (English)
            LANGUAGE_ZH,    // 简体中文 (中文)
            LANGUAGE_ZH_TW, // 繁体中文 (台灣)
            LANGUAGE_ZH_HK, // 繁体中文 (香港)
            LANGUAGE_JA,    // 日语 (日本語)
            LANGUAGE_KO,    // 韩语 (한국어)
            LANGUAGE_FR,    // 法语 (Français)
            LANGUAGE_DE,    // 德语 (Deutsch)
            LANGUAGE_ES,    // 西班牙语 (Español)
            LANGUAGE_RU,    // 俄语 (Русский)
            LANGUAGE_IT,    // 意大利语 (Italiano)
            LANGUAGE_PT,    // 葡萄牙语 (Português)
            LANGUAGE_AR,    // 阿拉伯语 (العربية)
            LANGUAGE_HI,    // 印地语 (हिन्दी)
            LANGUAGE_TH,    // 泰语 (ภาษาไทย)
            LANGUAGE_VI,    // 越南语 (Tiếng Việt)
            LANGUAGE_TR,    // 土耳其语 (Türkçe)
            LANGUAGE_NL,    // 荷兰语 (Nederlands)
            LANGUAGE_PL,    // 波兰语 (Polski)
            LANGUAGE_UK     // 乌克兰语 (Українська)
        };

    public:
        LanguageManager();
        virtual ~LanguageManager();
        static LanguageManager &getInstance();
        void addLanguage(Language language, const std::shared_ptr<Strings> &Strings);
        void removeLanguage(Language language);
        void setLanguage(Language language);
        Language getLanguage();
        std::string getString(const std::string &key);

    private:
        Language mLanguage = Language::LANGUAGE_EN;
        std::map<Language, std::shared_ptr<Strings>> mLanguageMap;
    };
}
#endif
