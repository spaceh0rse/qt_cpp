#include "ipv6.h"
#include "interface.h"
#include "os_specifier.h"
#include <iostream>
#include <QString>
#include <QProcess>

using namespace std;

/*
 *ONLY ipv4 support right now, implemention is next big feature
 *
 */

QString IPv6::user_in=" ";
QString IPv6::ownIP=" ";
int IPv6::counterSplit_cut=0,IPv6::responseDelay,IPv6::counter_devices=0;
QStringList IPv6::array;
string IPv6::delay;

void IPv6::run()
{
    string address;

    array.clear();

    split_cut(user_in.toStdString());

    int ip1 =array[0].toInt(); int ziel1 = array[4].toInt();
    int ip2 =array[1].toInt(); int ziel2 = array[5].toInt();
    int ip3 =array[2].toInt(); int ziel3 = array[6].toInt();
    int ip4 =array[3].toInt(); int ziel4 = array[7].toInt();

    while(!this->isInterruptionRequested()){

        ip4++;

            if(ip4==256){
                ip3++;
                ip4=1;
            }else if(ip3==256){
                ip2++;
                ip3=1;ip4=1;
            }else if(ip2==256){
                ip1++;
                ip2=1;ip3=1;ip4=1;
            }else if(ip1==256){
                break;
            }else if((ip1==ziel1 && ip2==ziel2 && ip3==ziel3) && ip4==ziel4+1){
                emit resultReady();
                break;
            }

            address = std::to_string(ip1)+"."+std::to_string(ip2)+"."+std::to_string(ip3)+"."+std::to_string(ip4);

            ping_address(QString::fromStdString(address));
    }
}

void IPv6::split_cut(string user_in){

    if(counterSplit_cut<3){

        string cut_left = user_in.substr(0,user_in.find_first_of('.'));
        string cut_right = user_in.substr(cut_left.length()+1,user_in.length());

        array.append(QString::fromStdString(cut_left));

        counterSplit_cut++;

        split_cut(cut_right);

    }else if(counterSplit_cut==3){

        string cut_left = user_in.substr(0,user_in.find_first_of('-'));
        string cut_right = user_in.substr(cut_left.length()+1,user_in.length());

        array.append(QString::fromStdString(cut_left));

        counterSplit_cut++;

        split_cut(cut_right);

    }else if(counterSplit_cut>3){

        if(counterSplit_cut==7){

            array.append(QString::fromStdString(user_in));
            counterSplit_cut=0;
            return;
        }

        string cut_left = user_in.substr(0,user_in.find_first_of('.'));
        string cut_right = user_in.substr(cut_left.length()+1,user_in.length());

        array.append(QString::fromStdString(cut_left));

        counterSplit_cut++;

        split_cut(cut_right);
    }
}

void IPv6::ping_address(QString address){

    OSSpecifier oss;
    QString eintragSuche,Rueckgabe_MAC, MAC = " ";
    int counter = 0;

    emit ipNow(address,counter_devices);

    if(QString::compare(address, ownIP)==0){

        cout << "LOOPBACK" << endl;

     }else{
        //build process
        QString befehl = "ping";
        QStringList parameter;
        parameter << QString::fromStdString(oss.pingParameter) << "1" << address;
        QProcess ping;

        //start process
        ping.start(befehl,parameter);

        //time-out
        ping.waitForFinished(responseDelay);

        //read output into QString
        QString Rueckgabe(ping.readAllStandardOutput());

        eintragSuche = " ";

            if(Rueckgabe.contains(QString::fromStdString(oss.macReturn))){

                //gettin MAC
                QString befehl_mac = QString::fromStdString(oss.arp)+address;
                QProcess MAC_beziehn;
                MAC_beziehn.start(befehl_mac);
                MAC_beziehn.waitForFinished(responseDelay);

                eintragSuche.append(address+"|");

                Rueckgabe_MAC = MAC_beziehn.readAllStandardOutput();
                QStringList reTurn_MAC = Rueckgabe_MAC.split(QRegExp("\\s+"),QString::SkipEmptyParts);

                foreach (QString entry, reTurn_MAC){
                    counter++;
                    if(counter==oss.MAC){
                        MAC = entry;
                    }
                }
                counter_devices++;
                eintragSuche.append(MAC+"|");
                emit entryTable(eintragSuche);
             }
        }
}
