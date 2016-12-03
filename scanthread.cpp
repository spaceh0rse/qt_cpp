#include "scanthread.h"
#include "os_specifier.h"
#include "interface.h"
#include <iostream>
#include <QString>
#include <QList>
#include <QStringList>
#include <QProcess>

using namespace std;

int scanThread::ip10=0;
int scanThread::ip20=0;
int scanThread::ip30=0;
int scanThread::ip40=0;
int scanThread::ip50=0;
QString scanThread::ownIP=" ";
bool scanThread::abort = false;

void scanThread::run() {

    std::string ip_start = std::to_string(ip10)+"."+std::to_string(ip20)+"."+std::to_string(ip30)+"."+std::to_string(ip40);
    std::string ip_end = std::to_string(ip10)+"."+std::to_string(ip20)+"."+std::to_string(ip30)+"."+std::to_string(ip50);

    int startAdresse = std::stoi(ip_start.substr(ip_start.find_last_of('.') + 1, ip_start.length()));
    int endAdresse = std::stoi(ip_end.substr(ip_end.find_last_of('.') + 1, ip_end.length()));;

    QString eigeneIP = ownIP;
    QString Bereich= QString::number(ip10)+"."+QString::number(ip20)+"."+QString::number(ip30)+".";

    QString Adress,eintragSuche,Rueckgabe_MAC,MAC_qstring,MAC_qvendor;
    string MAC_temp,MAC_final,MAC_vendor;

    for(startAdresse;startAdresse<=endAdresse;startAdresse++){

        //abort
        if(this->abort==true){
            break;
        }

        //Umwandlung muss in einem QString passieren, Umwandlung via std::to_string() wirft Exception
        Adress = QString::number(startAdresse);

        //Fallunterscheidung damit kein Ping an die eigene Adresse geschickt wird
        if(eigeneIP==Bereich+Adress){

            //LoopBACK

        }else{

            //if u dont use it==remove include iostream and namespace
            std::cout << Bereich.toStdString() << Adress.toStdString() << std::endl;

            QString befehl = "ping";
            QStringList parameter;
            parameter << "-n" << "1" << Bereich+Adress;
            QProcess ping;

            //Starten des Process mit Übergabe der Parameter
            ping.start(befehl,parameter);

            //Time-Out
            ping.waitForFinished(1000);

            //Lesen der Ausgabe in einen QString für die Weiterverarbeitung
            QString Rueckgabe(ping.readAllStandardOutput());

            eintragSuche = " ";

            if(Rueckgabe.contains("Antwort")){

                //MAC Adresse holn via Systemfunktionen...
                QString befehl_mac = "arp -a "+Bereich+Adress; //Funzt nur unter Windows
                QProcess MAC_beziehn;
                MAC_beziehn.start(befehl_mac);
                MAC_beziehn.waitForFinished(500);

                eintragSuche.append(Bereich+Adress+"|");

                Rueckgabe_MAC = MAC_beziehn.readAllStandardOutput();
                MAC_temp = Rueckgabe_MAC.toStdString();
                MAC_final = MAC_temp.substr(116,17);
                MAC_qstring = QString::fromStdString(MAC_final);

                eintragSuche.append(MAC_qstring+"|");

                //Vendor der MAC-Adresse ermitteln
                MAC_vendor = MAC_final.substr(0,8);
                MAC_qvendor = QString::fromStdString(MAC_vendor);

                eintragSuche.append(MAC_qvendor+"|");

                emit entryTable(eintragSuche);
             }
        }
        //MAGIC
        //signals for the gui like progressbar and tableview
        emit resultReady(startAdresse);
    }
}
