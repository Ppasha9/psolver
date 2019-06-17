#ifndef SOLVERPARAMSDIALOG_H
#define SOLVERPARAMSDIALOG_H

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QDeclarativeView)

namespace Ui {
    class SolverParamsDialog;
}

class SolverParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolverParamsDialog(QWidget *parent = 0);
    ~SolverParamsDialog();

    QDeclarativeView *& getSolverParamsView() const;

private:
    Ui::SolverParamsDialog *ui;
};

#endif // SOLVERPARAMSDIALOG_H
