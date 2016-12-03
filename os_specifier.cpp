#include "os_specifier.h"
#include <stdio.h>
#include <string>

using std::string;

string OSSpecifier::pingParameter;
string OSSpecifier::macReturn;

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

    #elif (defined (LINUX) || defined (__linux__))
        /*
         * Linux
         */

        // ping
        pingParameter = "-c";
        macReturn = "1 received";

    #endif
}
