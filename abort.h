#ifndef ABORT_H
#define ABORT_H

#include <QDialog>

namespace Ui {
class Abort;
}

class Abort : public QDialog
{
    Q_OBJECT

public:
    explicit Abort(QWidget *parent = 0);
    ~Abort();

private:
    Ui::Abort *ui;
};

#endif // ABORT_H
