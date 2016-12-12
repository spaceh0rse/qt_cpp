#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    void ini();
    QStringList getSetting();
    void setSetting();

    static QString sysDir;

private:
    Ui::Settings *ui_settings;

signals:
    void read(QStringList setting);

private slots:
    void on_buttonBox_accepted();
};

#endif // SETTINGS_H
