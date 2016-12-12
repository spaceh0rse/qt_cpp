#ifndef OS_SPECIFIER_H
#define OS_SPECIFIER_H

#include <string>

using std::string;

class OSSpecifier
{
public:
    OSSpecifier();
    static string pingParameter;
    static string macReturn;
    static string arp;
    static string lan;
    static string wlan;
    static int MAC;
};

#endif // OS_SPECIFIER_H
