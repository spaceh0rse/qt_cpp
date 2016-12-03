#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <network_manager.h>
#include <QStandardItemModel>
#include <iostream>
#include <string>
#include <QIntValidator>
#include <QLinkedList>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>

#include "scanthread.h"

using namespace std;

/*
 *TODO:
 *
 *              -übergabe von parameter an run function                             DONE
 *              -auslesen der eingabe von lineedit                                  DONE
 *              -check ob onEntryTable slot wirklich funzt (nicht wirklich)         DONE
 *              -shortcuts                                                          DONE
 *              -abbrechen des threads bzw. suchvorgang (bool abort signal)         JA, aber dann freeze
 *              -unix check aka os_specifier                                        ....
 *              -auslesen von eigener ip-adresse und weiteren angaben               DONE
 *              -check noch loopback-adresse                                        DONE
 *              -menubar(WHY)                                                       DONE
 *              -wlan shit                                                          DONE
 *              -clearn tablewidget(setrowcount==CRASH)                             ....
 *              -icon == check dir                                                  DONE
 *
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connection to our corresponding signal and slot, responding to button run
    connect(ui->btnRun, SIGNAL(clicked()), SLOT(run()));
    connect(ui->btnAbort, SIGNAL(clicked()), SLOT(abort()));


    //connect(MainWindow::run(), SIGNAL(abortThread()), SLOT(abort()));
    /*
     * Validierer fuer IP-Eingabeboxen
     * akzeptiert nur Zahlen von 1 bis 255
     *
     * Fehler bei Eingabe 00000...
    */
    QIntValidator *ip_1 = new QIntValidator(1,255);
    ui->lineEdit_ip1->setValidator(ip_1);

    QIntValidator *ip_2 = new QIntValidator(1,255);
    ui->lineEdit_ip2->setValidator(ip_2);

    QIntValidator *ip_3 = new QIntValidator(1,255);
    ui->lineEdit_ip3->setValidator(ip_3);

    QIntValidator *ip_4 = new QIntValidator(1,255);
    ui->lineEdit_ip4->setValidator(ip_4);

    QIntValidator *ip_5 = new QIntValidator(1,255);
    ui->lineEdit_ip5->setValidator(ip_5);    

    NetworkManager netMan;
    QList<NetScanInterface> ownNet = netMan.getOwnNetwork();

    string placeholder = ownNet[0].ip.toStdString();

    string p_cut = placeholder.substr(placeholder.find_first_of("."),placeholder.find_last_of("."));

    string Pos_1 = placeholder.substr(0,placeholder.find_first_of("."));
    string Pos_2 = placeholder.substr(placeholder.find_first_of(".")+1,placeholder.find_first_of("."));
    string Pos_3 = p_cut.substr(placeholder.find_first_of(".")+2,placeholder.find_first_of("."));

    QString pos_1 = QString::fromStdString(Pos_1);
    QString pos_2 = QString::fromStdString(Pos_2);
    QString pos_3 = QString::fromStdString(Pos_3);

    ui->lineEdit_ip1->setText(pos_1);
    ui->lineEdit_ip2->setText(pos_2);
    ui->lineEdit_ip3->setText(pos_3);

    //set labels with ownNetwork data
    ui->lbl_iface->setText(ownNet[0].iface);
    ui->lbl_ip->setText(ownNet[0].ip);
    ui->lbl_sub->setText(ownNet[0].sub);
    ui->lbl_hostname->setText(ownNet[0].hostname);
    ui->lbl_mac->setText(ownNet[0].mac);

}

//run function !!NOT the one from QThread!! some kind of container
void MainWindow::run() {

    QString ip1 = ui->lineEdit_ip1->text();
    QString ip2 = ui->lineEdit_ip2->text();
    QString ip3 = ui->lineEdit_ip3->text();
    QString ip4 = ui->lineEdit_ip4->text();
    QString ip5 = ui->lineEdit_ip5->text();

    QString ownIP = ui->lbl_ip->text();

    scanThread::ip10=ip1.toInt();
    scanThread::ip20=ip2.toInt();
    scanThread::ip30=ip3.toInt();
    scanThread::ip40=ip4.toInt();
    scanThread::ip50=ip5.toInt();
    scanThread::ownIP=ownIP;

    int min_p = ip4.toInt();
    int max_p = ip5.toInt();

    ui->progressBar->setMinimum(min_p);
    ui->progressBar->setMaximum(max_p);

    //new scanThread object == containing the overriden run-function from QThread
    scanThread * worker = new scanThread;
    //connect signal and corresponding slot
    connect(worker, SIGNAL(resultReady(int)), SLOT(onResultReady(int)));
    //connect signal to slot for the table_count and entry which shall be append to the table
    connect(worker, SIGNAL(entryTable(QString)),SLOT(onEntryTable(QString)));
    //connect signal and slot for clean up when finished
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

        //starting computation in another thread
        worker->start();

}




//slot-function to update the progressbar
void MainWindow::onResultReady(int res) {

    QString ip5 = ui->lineEdit_ip5->text();
    int done = ip5.toInt();

    //update the progressbar
    ui->progressBar->setValue(res);
    ui->progressBar->setTextVisible(true);
    ui->progressBar->setAlignment(Qt::AlignCenter);
    if(res==done){
        ui->progressBar->setFormat("Scanning finished");
    }
}

//slot-function to draw the tablewidget
void MainWindow::onEntryTable(QString entry){

    //get entry and save into list
    listEintraege.append(entry);
    //insert row into tablewidget
    ui->tableWidget->insertRow(0);

    int counter = 0;

    foreach(QString eintrag, listEintraege){

        string Auslese_temp=eintrag.toStdString();

        string ip = Auslese_temp.substr(0,Auslese_temp.find_first_of("|"));
        string mac = Auslese_temp.substr(Auslese_temp.find_first_of("|")+1,17);

        QString ipAdresse = QString::fromStdString(ip);
        QString macAdresse = QString::fromStdString(mac);

        ui->tableWidget->setItem(counter,0,new QTableWidgetItem("X"));
        ui->tableWidget->setItem(counter,1,new QTableWidgetItem(ipAdresse));
        ui->tableWidget->setItem(counter,2,new QTableWidgetItem(macAdresse));

        ui->tableWidget->item(counter,0)->setBackground(Qt::green);
        ui->tableWidget->item(counter,1)->setBackground(Qt::green);
        ui->tableWidget->item(counter,2)->setBackground(Qt::green);

        ui->tableWidget->item(counter,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(counter,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(counter,2)->setTextAlignment(Qt::AlignCenter);
        counter++;
    }
}

//application close function
MainWindow::~MainWindow()
{
    delete ui;
}

// !!!DEBUG!!!
//slot-function to abort the process
void MainWindow::abort()
{
    //let`s see
    //emit(abortThread(true));

    //scanThread::abort=true;

    //clear lineedit und tablewidget
    //ui->lineEdit_ip4->clear();
    //ui->lineEdit_ip5->clear();

    //CRASH!!!!!!!!!!!!!!!!
    //ui->tableWidget->clear();
    //ui->progressBar->setFormat(" ");
    //ui->progressBar->setMinimum(1);
    //ui->progressBar->setMaximum(1);

    QPrinter printer;
    QPrintDialog printer_dialog(&printer);
    if (printer_dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        ui->tableWidget->render(&painter);
    }

}

