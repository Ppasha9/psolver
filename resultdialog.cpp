#include "resultdialog.h"
#include "ui_resultdialog.h"

#include "Interfaces/IVector.h"
#include "Interfaces/ILog.h"

#define REPORT(MSG) \
    QString qmsg("[CONTROLLER_ResultDialog]:  "); \
    qmsg += QString(MSG); \
    qmsg += "\n\t\tFile: "; \
    qmsg += __FILE__; \
    qmsg += "\n\t\tLine: "; \
    qmsg += QString::number(__LINE__); \
    qmsg += "\n\t\tFunction: "; \
    qmsg += __FUNCTION__; \
    ILog::report(qmsg.toStdString().c_str())


ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
    QString title = "Result of solving";
    setWindowTitle(title);
}

int ResultDialog::setSolution(IVector * res)
{
    double x;
    ErrorEnum err;
    size_t dim;

    dim = res->getDim();
    QStringList resStrList;
    for (unsigned int i = 0; i < dim; i++)
    {
        err = (ErrorEnum)res->getCoord(i, x);
        if (err != ERR_OK)
        {
            REPORT("Cannot get " + QString::number(i) + " coordinate from result vector.");
            return err;
        }

        resStrList.append(QString::number(x));
    }

    QString resStr = resStrList.join(", ");
    ui->lineEdit->setText(resStr);

    return ERR_OK;
}

ResultDialog::~ResultDialog()
{
    delete ui;
}
