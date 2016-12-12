#ifndef DONE_H
#define DONE_H

#include <QDialog>

namespace Ui {
class Done;
}

class Done : public QDialog
{
    Q_OBJECT

public:
    explicit Done(QWidget *parent = 0);
    ~Done();

private:
    Ui::Done *ui_done;
};

#endif // DONE_H
