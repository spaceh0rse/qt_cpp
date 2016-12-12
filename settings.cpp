#include "settings.h"
#include "ui_settings.h"
#include <iostream>
#include <QSettings>
#include <qDebug>
#include <QApplication>

#include "addressrange.h"

using namespace std;

QString Settings::sysDir= " ";

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui_settings(new Ui::Settings)
{
    ui_settings->setupUi(this);
    ui_settings->pic_settings->setPixmap(QApplication::applicationDirPath()+"/pic/settings.png");
    ui_settings->pic_settings->setAlignment(Qt::AlignCenter);
    sysDir = QApplication::applicationDirPath();

    getSetting();
}

Settings::~Settings()
{
    delete ui_settings;
}


void Settings::ini()
{

}

QStringList Settings::getSetting()
{
    QSettings settings(sysDir+"/settings.ini", QSettings::IniFormat);
    settings.beginGroup("/SYSTEM.SETTING");
    const QStringList childKeys = settings.childKeys();
    QStringList values;
    foreach (const QString &childKey, childKeys)
    {
        values << settings.value(childKey).toString();
        ui_settings->lineEditDelay->setText(settings.value(childKey).toString());

    }

    settings.endGroup();
    return values;
}

void Settings::setSetting()
{
    QSettings settings(sysDir+"/settings.ini", QSettings::IniFormat);
    settings.beginGroup("/SYSTEM.SETTING");
    QString save = ui_settings->lineEditDelay->text();
    settings.setValue("Delay", save);
    settings.endGroup();
    return;
}

void Settings::on_buttonBox_accepted()
{
    setSetting();
}
