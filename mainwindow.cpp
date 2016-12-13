#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "addressrange.h"
#include "ipv6.h"
#include "done.h"
#include "about.h"
#include "abort.h"
#include <network_manager.h>
#include <QStandardItemModel>
#include <iostream>
#include <string>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QIntValidator>
#include <QRegExp>

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

    QRegExp format("(|-|\\.|[0-9]){31}");
    QRegExpValidator *ip_Exp = new QRegExpValidator(format, this);
    ui->lineEdit_addressRange->setValidator(ip_Exp);

    //set labels with ownNetwork data
    ui->lbl_iface->setText(ownNet[0].iface);
    ui->lbl_ip->setText(ownNet[0].ip);
    ui->lbl_sub->setText(ownNet[0].sub);
    ui->lbl_hostname->setText(ownNet[0].hostname);
    ui->lbl_mac->setText(ownNet[0].mac);

    ui->Background->setPixmap(QApplication::applicationDirPath()+"/pic/server.png");
    ui->Background->setAlignment(Qt::AlignCenter);
    setWindowIcon(QIcon(QApplication::applicationDirPath()+"/pic/server.png"));

}

//run function !!NOT the one from QThread!! some kind of container
void MainWindow::run() {

    worker_ipv4 = new addressRange;
    worker_ipv6 = new IPv6;

    Settings set;
    QStringList settings = set.getSetting();
    QString delay = settings.last();

    addressRange::ownIP=ui->lbl_ip->text();
    addressRange::user_in=ui->lineEdit_addressRange->text();
    addressRange::responseDelay=delay.toInt();
    addressRange::counter_devices=0;

    ui->tableWidget->setRowCount(0);
    ui->label_feedback_count->setText("0");
    ui->progressBar->setMinimum(1);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setTextVisible(true);
    ui->progressBar->setAlignment(Qt::AlignCenter);
    ui->progressBar->setFormat("Scanning");

    //connect signals and corresponding slots
    connect(worker_ipv4, SIGNAL(resultReady()), SLOT(onResultReady()));
    connect(worker_ipv4, SIGNAL(entryTable(QString)),SLOT(onEntryTable(QString)));
    connect(worker_ipv4, SIGNAL(ipNow(QString,int)),SLOT(ipFeedback(QString,int)));
    connect(worker_ipv4, SIGNAL(finished()), worker_ipv4, SLOT(deleteLater()));
    //starting computation in another thread

    worker_ipv4->start();
}

void MainWindow::fillTable(QStringList addresses){

    int table_counter = 0;

    foreach(QString entry, addresses){

        string temp=entry.toStdString();

        string ip = temp.substr(0,temp.find_first_of("|"));
        string mac = temp.substr(temp.find_first_of("|")+1,17);

        QString ipAdress = QString::fromStdString(ip);
        QString macAdress = QString::fromStdString(mac);

        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->tableWidget->setItem(table_counter,0,new QTableWidgetItem("X"));
        ui->tableWidget->setItem(table_counter,1,new QTableWidgetItem(ipAdress));
        ui->tableWidget->setItem(table_counter,2,new QTableWidgetItem(macAdress));

        ui->tableWidget->item(table_counter,0)->setBackground(Qt::green);
        ui->tableWidget->item(table_counter,1)->setBackground(Qt::green);
        ui->tableWidget->item(table_counter,2)->setBackground(Qt::green);

        ui->tableWidget->item(table_counter,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(table_counter,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(table_counter,2)->setTextAlignment(Qt::AlignCenter);
        table_counter++;
    }
    addresses.clear();
}

//slot-function to update the progressbar
void MainWindow::onResultReady() {

    Done complet;
    complet.exec();
    ui->progressBar->setMaximum(1);
    ui->progressBar->setTextVisible(false);
    ui->label_feedback_output->setText("Finished");
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

    QPrinter printer;
    QPrintDialog printer_dialog(&printer);
    if (printer_dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        ui->tableWidget->render(&painter);
    }    
}

//slot-function for user_feedback
void MainWindow::ipFeedback(QString ip, int count){
    ui->label_feedback_output->setText(ip);
    ui->label_feedback_count->setText(QString::number(count));
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

void MainWindow::on_btnAbort_clicked()
{
    if (worker_ipv4 != false && worker_ipv4->isRunning()) {
        worker_ipv4->requestInterruption();
        worker_ipv4->wait();

        ui->progressBar->setMaximum(1);
        ui->progressBar->setTextVisible(false);
        ui->label_feedback_output->setText("Scan canceled ...");
        ui->lineEdit_addressRange->setText(" ");
        ui->tableWidget->setRowCount(addressRange::counter_devices);
        ui->label_feedback_count->setText("0");

        Abort stop;
        stop.exec();

    }else{

    }
}
