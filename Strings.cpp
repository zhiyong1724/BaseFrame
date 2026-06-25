#include "Strings.h"
namespace BaseFrame
{
    Strings::Strings()
    {
    }

    Strings::~Strings()
    {
    }

    std::string Strings::getString(const std::string &key)
    {
        auto itr = mStringsMap.find(key);
        if (itr != mStringsMap.end())
        {
            return itr->second;
        }
        return "";
    }
}