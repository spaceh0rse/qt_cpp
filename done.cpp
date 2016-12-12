#include "done.h"
#include "ui_done.h"

Done::Done(QWidget *parent) :
    QDialog(parent),
    ui_done(new Ui::Done)
{
    ui_done->setupUi(this);
    ui_done->pic_ok->setPixmap(QApplication::applicationDirPath()+"/pic/done.png");
    ui_done->pic_ok->setAlignment(Qt::AlignCenter);
    setWindowIcon(QIcon(QApplication::applicationDirPath()+"/pic/server.png"));
}

Done::~Done()
{    
    delete ui_done;
}
