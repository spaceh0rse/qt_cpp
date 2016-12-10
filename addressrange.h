#ifndef ADDRESSRANGE_H
#define ADDRESSRANGE_H

#include <QThread>
#include <QString>

using namespace std;

class addressRange : public QThread
{
    Q_OBJECT
public:
    explicit addressRange(QObject *parent = 0) : QThread(parent) {

    }
    void run();
    void split_cut(string user_in);
    void ping_address(QString address);

    QStringList ipArray;

    static string delay;
    static int counterSplit_cut,responseDelay;
    static QString user_in, ownIP;
    static QStringList array;

signals:
    void resultReady();
    void entryTable(QString);
    void ipNow(QString);
    void done();
};

#endif // ADDRESSRANGE_H
