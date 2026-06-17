#ifndef __STRINGS_H__
#define __STRINGS_H__
#include <string>
#include <map>
namespace BaseFrame
{
    class Strings
    {
    public:
        Strings(/* args */);
        virtual ~Strings();
        virtual std::string getString(const std::string &key);

    protected:
        std::map<std::string, std::string> mStringsMap;
    };
}
#endif