#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->solveButton(), SIGNAL(clicked()), this, SLOT(onSolve()));
    connect(ui->actionAdd_solver(), SIGNAL(triggered()), this, SLOT(onAddSolver()));
    connect(ui->actionChoose_problem(), SIGNAL(triggered()), this, SLOT(onChooseTask()));

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSolve()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

    if (!selectionModel->hasSelection())
    {
        statusBar()->showMessage("No solver selected!");
        return;
    }

    QModelIndexList indexList = selectionModel->selectedRows();

    // Get path from indexList.front();

    QString path;

    if (!loadSolver(path))
        return;

    // Show parameters dialogue

    solve();

    // Show result window
}

void MainWindow::onChooseProblem()
{
    QString path = QFileDialog.getOpenFileName(0, "Choose problem file", QString(), "*.dll");
    loadProblem(path);
}

void MainWindow::onAddSolver()
{
    QString path = QFileDialog.getOpenFileName(0, "Choose solver file", QString(), "*.dll");
    QString name = QInputDialog.getText(0, "Input solver name");

    // Add solver to database
}
