#include "addressrange.h"
#include "interface.h"
#include <iostream>
#include <QString>
#include <QProcess>

using namespace std;

QString addressRange::user_in=" ";
QString addressRange::ownIP=" ";
int addressRange::counterSplit_cut=0;
QStringList addressRange::array;

void addressRange::run(){

    string address;

    split_cut(user_in.toStdString());

    int ip1 =array[0].toInt(); int ziel1 = array[4].toInt();
    int ip2 =array[1].toInt(); int ziel2 = array[5].toInt();
    int ip3 =array[2].toInt(); int ziel3 = array[6].toInt();
    int ip4 =array[3].toInt(); int ziel4 = array[7].toInt();

    while(true){

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

            }else if((ip1==ziel1 && ip2==ziel2 && ip3==ziel3) && ip4==ziel4+1){
                break;
            }

            address = std::to_string(ip1)+"."+std::to_string(ip2)+"."+std::to_string(ip3)+"."+std::to_string(ip4);

            ping_address(QString::fromStdString(address));
            }
}

void addressRange::split_cut(string user_in){

    if(counterSplit_cut<3){

        string cut_left = user_in.substr(0,user_in.find_first_of('.'));
        string cut_right = user_in.substr(sizeof(cut_left),user_in.length());

        array.append(QString::fromStdString(cut_left));

        counterSplit_cut++;

        split_cut(cut_right);

    }else if(counterSplit_cut==3){

        string cut_left = user_in.substr(0,user_in.find_first_of('-'));
        string cut_right = user_in.substr(sizeof(cut_left),user_in.length());

        array.append(QString::fromStdString(cut_left));

        counterSplit_cut++;

        split_cut(cut_right);

    }else if(counterSplit_cut>3){

        if(counterSplit_cut==7){

            array.append(QString::fromStdString(user_in));

            return;
        }

        string cut_left = user_in.substr(0,user_in.find_first_of('.'));
        string cut_right = user_in.substr(sizeof(cut_left),user_in.length());

        array.append(QString::fromStdString(cut_left));

        counterSplit_cut++;

        split_cut(cut_right);
    }
}

void addressRange::ping_address(QString address){

    QString eintragSuche,Rueckgabe_MAC,MAC_qstring;
    string MAC_temp,MAC_final;

    emit ipNow(address);

    if(QString::compare(address, ownIP)==0){

        cout << "LOOPBACK" << endl;

     }else{

        //bulid process
        QString befehl = "ping";
        QStringList parameter;
        parameter << "-n" << "1" << address;
        QProcess ping;

        //start process
        ping.start(befehl,parameter);

        //time-out
        ping.waitForFinished(500);

        //read output into QString
        QString Rueckgabe(ping.readAllStandardOutput());

        eintragSuche = " ";

            if(Rueckgabe.contains("Antwort")){

                //gettin MAC
                QString befehl_mac = "arp -a "+address; //Funzt nur unter Windows
                QProcess MAC_beziehn;
                MAC_beziehn.start(befehl_mac);
                MAC_beziehn.waitForFinished(500);

                eintragSuche.append(address+"|");

                Rueckgabe_MAC = MAC_beziehn.readAllStandardOutput();
                MAC_temp = Rueckgabe_MAC.toStdString();
                MAC_final = MAC_temp.substr(116,17);
                MAC_qstring = QString::fromStdString(MAC_final);

                eintragSuche.append(MAC_qstring+"|");

                emit entryTable(eintragSuche);
             }
        }
        emit resultReady('1');
}
