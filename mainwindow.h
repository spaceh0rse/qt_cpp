#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_settings.h"
#include "ui_about.h"

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

private:
    Ui::MainWindow *ui;
    QDialog *formSettings;
    QDialog *formAbout;
    Ui::Settings Form_settings;
    Ui::About Form_about;
    QStringList listEntry;

private slots:
    void run();
    void onResultReady(int res);
    void onEntryTable(QString entry);
    void ipFeedback(QString address);
    void print();
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();

signals:

};

#endif // MAINWINDOW_H
