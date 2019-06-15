#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSolve()
{
    //TODO: ...
}

void MainWindow::onChooseTask(const QString &path)
{
    //TODO: ...
}

void MainWindow::onAddSolver()
{
    //TODO: ...
}

