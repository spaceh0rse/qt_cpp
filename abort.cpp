#include "abort.h"
#include "ui_abort.h"

Abort::Abort(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Abort)
{
    ui->setupUi(this);
    ui->pic_abort->setPixmap(QApplication::applicationDirPath()+"/pic/abort.png");
    ui->pic_abort->setAlignment(Qt::AlignCenter);
    setWindowIcon(QIcon(QApplication::applicationDirPath()+"/pic/server.png"));
}

Abort::~Abort()
{
    delete ui;
}
