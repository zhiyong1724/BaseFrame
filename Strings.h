#ifndef __STRINGS_H__
#define __STRINGS_H__
#include <string>
namespace BaseFrame 
{
    class Strings
    {
    public:
        Strings(/* args */) {}
        virtual ~Strings() {}
        virtual std::string getString(const std::string &key) = 0;
    };
}
#endif