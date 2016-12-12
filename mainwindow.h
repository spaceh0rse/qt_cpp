#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_settings.h"
#include "ui_about.h"

#include "addressrange.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void fillTable(QStringList addresses);
    ~MainWindow();

    addressRange * worker = new addressRange;

private:
    Ui::MainWindow *ui;
    QDialog *formSettings;
    QDialog *formAbout;
    QStringList listEntry;

    static int settingDelay;

private slots:
    void run();
    void onResultReady();
    void onEntryTable(QString entry);
    void ipFeedback(QString address);
    void print();
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_btnAbort_clicked();
};

#endif // MAINWINDOW_H
