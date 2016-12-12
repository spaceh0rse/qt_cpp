#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "addressrange.h"
#include "done.h"
#include "about.h"
#include <network_manager.h>
#include <QStandardItemModel>
#include <iostream>
#include <string>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>




/*
 * Reset
 * Abort
 * String
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

    Settings set;
    QStringList settings = set.getSetting();
    QString delay = settings.last();

    addressRange::ownIP=ui->lbl_ip->text();
    addressRange::user_in=ui->lineEdit_addressRange->text();
    addressRange::responseDelay=delay.toInt();

    cout << ui->lineEdit_addressRange->text().toStdString() << endl;

    ui->tableWidget->clearContents();

    ui->progressBar->setMinimum(1);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setTextVisible(true);
    ui->progressBar->setAlignment(Qt::AlignCenter);
    ui->progressBar->setFormat("Scanning");

    //new scanThread object == containing the overriden run-function from QThread


    //connect signals and corresponding slots
    connect(worker, SIGNAL(resultReady()), SLOT(onResultReady()));
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

        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //ui->tableWidget->horizontalHeader()->setDefaultSectionSize(200);

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
    addresses.clear();
}

//slot-function to update the progressbar
void MainWindow::onResultReady() {

    Done complet;
    //connect(Done::complet, SIGNAL(clearGUI()), SLOT(onclearGUI()));
    complet.exec();
    ui->progressBar->setMaximum(1);
    ui->progressBar->setTextVisible(false);
}

//slot-function to draw the tablewidget
void MainWindow::onEntryTable(QString entry){

    //get entry and save into list
    listEntry.append(entry);
    //insert row into tablewidget
    ui->tableWidget->insertRow(0);
    //fill tablewidget with entrys
    fillTable(listEntry);
    listEntry.clear();
}

//application close function
MainWindow::~MainWindow(){
    delete ui;
}

//slot-function to abort the process
void MainWindow::print(){

    cout << "lol" << endl;

    if (worker != false && worker->isRunning()) {
        worker->requestInterruption();
        worker->wait();
    }


    /*
    if (worker != false && worker->isRunning()) {
        worker->requestInterruption();
        worker->wait();
    }


    QPrinter printer;
    QPrintDialog printer_dialog(&printer);
    if (printer_dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        ui->tableWidget->render(&painter);
    }
    */
}

//slot-function for user_feedback
void MainWindow::ipFeedback(QString ip){
    ui->label_feedback_output->setText(ip);
}

//slot-function for the menubar
void MainWindow::on_actionSettings_triggered()
{
    Settings option;
    option.exec();
}

//slot-function for the menubar
void MainWindow::on_actionAbout_triggered()
{
    About us;
    us.exec();
}

//slot-function for the menubar
void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

void MainWindow::onclearGUI()
{
    cout << "TEST" << endl;
}
