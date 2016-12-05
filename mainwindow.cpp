#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <network_manager.h>
#include <QStandardItemModel>
#include <iostream>
#include <string>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>

#include "addressrange.h"

/*
 * TODO:            OSspecifier
 *                  Abbrechen?
 *                  Settings
 *                  Languages
 *                  debug 256?
 */

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnRun, SIGNAL(clicked()), SLOT(run()));
    connect(ui->btnPrint, SIGNAL(clicked()), SLOT(print()));

    NetworkManager netMan;
    QList<NetScanInterface> ownNet = netMan.getOwnNetwork();

    //set labels with ownNetwork data
    ui->lbl_iface->setText(ownNet[0].iface);
    ui->lbl_ip->setText(ownNet[0].ip);
    ui->lbl_sub->setText(ownNet[0].sub);
    ui->lbl_hostname->setText(ownNet[0].hostname);
    ui->lbl_mac->setText(ownNet[0].mac);

}

//run function !!NOT the one from QThread!! some kind of container
void MainWindow::run() {

    QString ownIP = ui->lbl_ip->text();
    addressRange::ownIP=ownIP;
    addressRange::user_in=ui->lineEdit_addressRange->text();

    int min_p = 0;
    int max_p = 100;

    ui->progressBar->setMinimum(min_p);
    ui->progressBar->setMaximum(max_p);

    //new scanThread object == containing the overriden run-function from QThread
    addressRange * worker = new addressRange;

    //connect signals and corresponding slots
    connect(worker, SIGNAL(resultReady(int)), SLOT(onResultReady(int)));   
    connect(worker, SIGNAL(entryTable(QString)),SLOT(onEntryTable(QString)));
    connect(worker, SIGNAL(ipNow(QString)),SLOT(ipFeedback(QString)));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //starting computation in another thread

    worker->start();
}

void MainWindow::fillTable(QStringList addresses){

    int counter = 0;

    foreach(QString entry, addresses){

        string temp=entry.toStdString();

        string ip = temp.substr(0,temp.find_first_of("|"));
        string mac = temp.substr(temp.find_first_of("|")+1,17);

        QString ipAdress = QString::fromStdString(ip);
        QString macAdress = QString::fromStdString(mac);

        ui->tableWidget->setItem(counter,0,new QTableWidgetItem("X"));
        ui->tableWidget->setItem(counter,1,new QTableWidgetItem(ipAdress));
        ui->tableWidget->setItem(counter,2,new QTableWidgetItem(macAdress));

        ui->tableWidget->item(counter,0)->setBackground(Qt::green);
        ui->tableWidget->item(counter,1)->setBackground(Qt::green);
        ui->tableWidget->item(counter,2)->setBackground(Qt::green);

        ui->tableWidget->item(counter,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(counter,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(counter,2)->setTextAlignment(Qt::AlignCenter);
        counter++;
    }
}

//slot-function to update the progressbar
void MainWindow::onResultReady(int res) {

    //QString ip5 = ui->lineEdit_ip5->text();
    int done = '10';

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
    listEntry.append(entry);
    //insert row into tablewidget
    ui->tableWidget->insertRow(0);
    //fill tablewidget with entrys
    fillTable(listEntry);
}

//application close function
MainWindow::~MainWindow(){
    delete ui;
}

//slot-function to abort the process
void MainWindow::print(){

    QPrinter printer;
    QPrintDialog printer_dialog(&printer);
    if (printer_dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        ui->tableWidget->render(&painter);
    }
}

//slot-function for user_feedback
void MainWindow::ipFeedback(QString ip){
    ui->label_feedback_output->setText(ip);
}

//slot-function for the menubar
void MainWindow::on_actionSettings_triggered()
{
    formSettings =new QDialog;
    formSettings->setModal(true);
    Form_settings.setupUi(formSettings);
    formSettings->show();
}

//slot-function for the menubar
void MainWindow::on_actionAbout_triggered()
{
    formAbout =new QDialog;
    formAbout->setModal(true);
    Form_about.setupUi(formAbout);
    formAbout->show();
}
