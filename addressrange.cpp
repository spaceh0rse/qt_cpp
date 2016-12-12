#include "addressrange.h"
#include "interface.h"
#include "os_specifier.h"
#include <iostream>
#include <QString>
#include <QProcess>

using namespace std;

QString addressRange::user_in=" ";
QString addressRange::ownIP=" ";
int addressRange::counterSplit_cut=0,addressRange::responseDelay;
QStringList addressRange::array;
string addressRange::delay;

void addressRange::run(){

    string address;

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

void addressRange::split_cut(string user_in){

    //user_input kommt immer an...keine Ahnung warum der reset nich funzt!

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

            return;
        }

        string cut_left = user_in.substr(0,user_in.find_first_of('.'));
        string cut_right = user_in.substr(cut_left.length()+1,user_in.length());

        array.append(QString::fromStdString(cut_left));

        counterSplit_cut++;

        split_cut(cut_right);
    }
}

void addressRange::ping_address(QString address){

    OSSpecifier oss;
    QString eintragSuche,Rueckgabe_MAC,MAC_qstring;
    string MAC_temp,MAC_final;

    emit ipNow(address);

    if(QString::compare(address, ownIP)==0){

        cout << "LOOPBACK" << endl;

     }else{

        //bulid process
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

        //std::cout << Rueckgabe.toStdString() << std::endl;

            if(Rueckgabe.contains(QString::fromStdString(oss.macReturn))){

                //gettin MAC
                QString befehl_mac = QString::fromStdString(oss.arp)+address; //Funzt nur unter Windows
                QProcess MAC_beziehn;
                MAC_beziehn.start(befehl_mac);
                MAC_beziehn.waitForFinished(responseDelay);

                eintragSuche.append(address+"|");

                Rueckgabe_MAC = MAC_beziehn.readAllStandardOutput();
                MAC_temp = Rueckgabe_MAC.toStdString();
                MAC_final = MAC_temp.substr(oss.MAC_1,oss.MAC_2);
                MAC_qstring = QString::fromStdString(MAC_final);

                //std::cout << MAC_temp << Rueckgabe_MAC.count(QLatin1Char(' ')) << std::endl;

                eintragSuche.append(MAC_qstring+"|");

                emit entryTable(eintragSuche);
             }
        }
}
