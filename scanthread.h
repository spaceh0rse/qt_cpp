#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QList>
#include <QString>
#include <QThread>


class scanThread: public QThread
{
    Q_OBJECT
public:
    explicit scanThread(QObject *parent = 0) : QThread(parent) {

    }
    void setArg(int a){int m_arg = a;}
    void run();
    static int ip10,ip20,ip30,ip40,ip50;
    static QString ownIP;
    static bool abort;

signals:
    void resultReady(int);
    void entryTable(QString);
};

#endif // SCANTHREAD_H
