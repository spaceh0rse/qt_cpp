#include "os_specifier.h"
#include <stdio.h>
#include <string>

using std::string;

string OSSpecifier::wlan,OSSpecifier::lan,OSSpecifier::arp,OSSpecifier::macReturn,OSSpecifier::pingParameter;
int OSSpecifier::MAC_1,OSSpecifier::MAC_2;


/*
 * Constructor
 *
 * put here more specific OS parameter
 *      --> declare above!
 */
OSSpecifier::OSSpecifier()
{
    #if (defined (_WIN32) || defined (_WIN64))
        /*
         * Windows
         */

        // ping
        pingParameter = "-n";
        macReturn = "Antwort";
        arp = "arp -a ";
        lan = "LAN";
        wlan = "WLAN";
        MAC_1 = 116;
        MAC_2 = 17;


    #elif (defined (LINUX) || defined (__linux__))
        /*
         * Linux
         */

        // ping
        pingParameter = "-c";
        macReturn = "1 received";
        arp = "arp";
        lan = "eth";
        wlan = "WLAN";
        MAC_1 = 29;
        MAC_2 = 18;

    #endif
}
