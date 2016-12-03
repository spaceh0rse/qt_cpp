#ifndef INTERFACE_H
#define INTERFACE_H

#include <QString>

using std::string;

class NetScanInterface
{
public:
    NetScanInterface();

    static QString iface;
    static QString ip;
    static QString sub;
    static QString hostname;
    static QString mac;

};

#endif // INTERFACE_H
