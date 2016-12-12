#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui_about(new Ui::About)
{
    ui_about->setupUi(this);
    //ui_about->pic_about->setVisible(true);
    ui_about->pic_about->setPixmap(QApplication::applicationDirPath()+"/pic/about.png");
    ui_about->pic_about->setAlignment(Qt::AlignCenter);
    setWindowIcon(QIcon(QApplication::applicationDirPath()+"/pic/server.png"));

    //ui_about->pic_about->show();
}

About::~About()
{
    delete ui_about;
}
