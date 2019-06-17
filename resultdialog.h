#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>

namespace Ui {
    class ResultDialog;
}

QT_FORWARD_DECLARE_CLASS(IVector)

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = 0);
    ~ResultDialog();    

    int setSolution(IVector * res);

private:
    Ui::ResultDialog *ui;
};

#endif // RESULTDIALOG_H
