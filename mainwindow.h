#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scanthread.h"
#include <QLinkedList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLinkedList<QString> listEintraege;

private slots:
    void run();
    void onResultReady(int res);
    void onEntryTable(QString entry);
    void abort();

signals:
    void abortThread(bool stop);
};

#endif // MAINWINDOW_H
