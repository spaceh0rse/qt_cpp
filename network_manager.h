#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "interface.h"
#include <string>
#include <QList>
#include <QString>
#include <QNetworkInterface>

using std::string;

class NetworkManager
{
public:
    NetworkManager();

    QList<NetScanInterface> getOwnNetwork();

    QList<QString> ownNetwork;    
    QNetworkInterface * network_interface;

    static string ipv4;
    static string sub;
    static string hostname;
    static string mac;

};

#endif // NETWORK_MANAGER_H
