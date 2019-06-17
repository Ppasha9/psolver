#include "solverparamsdialog.h"
#include "ui_solverparamsdialog.h"

SolverParamsDialog::SolverParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolverParamsDialog)
{
    ui->setupUi(this);
    QString title = "Solver's parameters";
    setWindowTitle(title);
}

SolverParamsDialog::~SolverParamsDialog()
{
    delete ui;
}

QDeclarativeView *& SolverParamsDialog::getSolverParamsView() const
{
    return ui->solverParamsView;
}

