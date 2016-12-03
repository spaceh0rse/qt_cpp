#include "network_manager.h"
#include "os_specifier.h"
#include "interface.h"
#include <QStringList>
#include <QNetworkAddressEntry>
#include <QAbstractSocket>
#include <QHostInfo>


string NetworkManager::ipv4;
string NetworkManager::sub;
string NetworkManager::hostname;
string NetworkManager::mac;


NetworkManager::NetworkManager()
{

}

// returns a list of network data about the own network
QList<NetScanInterface> NetworkManager::getOwnNetwork(){

    //check wether to use the ethernet or wlan apdater
    network_interface = new QNetworkInterface();
    foreach (QNetworkInterface interface, network_interface->allInterfaces()) {
            if((interface.flags() & QNetworkInterface::IsUp) && (interface.flags() & QNetworkInterface::IsRunning)){
                if(interface.humanReadableName().contains("LAN")||interface.humanReadableName().contains("WLAN")){

                    // return QList<Interface> --> test
                    QList<NetScanInterface> list;

                    // get all interfaces
                    QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();

                    // Iteration über alle Interface welche dem System bekannt sind
                    foreach(QNetworkInterface iface, allInterfaces) {

                        // Weitere Schleife und Iteration um von den Interfaces die einzelnen Einträge zu bekommen
                        QList<QNetworkAddressEntry> allEntries = iface.addressEntries();

                        foreach (QNetworkAddressEntry entry, allEntries) {

                            // keine LoopBack Adressen...localhost, 127.0.0.1
                            if(!entry.ip().isLoopback()){

                                // IP-Adress muss im IPv4 Protocoll vorliegen
                                if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol && iface.hardwareAddress() == interface.hardwareAddress()) {

                                    // new interface von interface.cpp
                                    NetScanInterface ns_interface;

                                    ns_interface.iface = iface.name();
                                    ns_interface.ip = entry.ip().toString();
                                    ns_interface.sub = entry.netmask().toString();
                                    ns_interface.hostname = QHostInfo::localHostName();
                                    ns_interface.mac = iface.hardwareAddress();

                                    list.append(ns_interface);
                                }
                            }
                        }
                    }
                    return list;
                }
            }
    }
}

