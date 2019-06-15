#include <QLibrary>

#include "Interfaces/IBrocker.h"
#include "Interfaces/ISolver.h"
#include "Interfaces/IProblem.h"

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

void MainWindow::addConnection()
{
    loadSolver("lol.kek");
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
}

bool MainWindow::loadSolver(const QString &path)
{
    QLibrary libSolver(path);

    bool loadResult = libSolver.load();
    if (!loadResult)
    {
        QString message;
        message.sprintf("Couldn't load solver library: '%s'", path);
        statusBar()->showMessage(message);
        return false;
    }

    m_ptrSolverBrocker = reinterpret_cast<IBrocker *>(libSolver.resolve("getBrocker"));
    if (!m_ptrSolverBrocker)
    {
        QString message;
        message.sprintf("Couldn't resolve 'getBrocker' function from library: '%s'", path);
        statusBar()->showMessage(path);
        return false;
    }

    m_ptrSolver = reinterpret_cast<ISolver *>(m_ptrSolverBrocker->getInterfaceImpl(IBrocker::SOLVER));
    if (!m_ptrSolver)
    {
        QString message;
        message.sprintf("Couldn't get 'ISolver' implementation from library: '%s'", path);
        statusBar()->showMessage(path);
        return false;
    }

    return true;
}

bool MainWindow::loadProblem(const QString &path)
{
    QLibrary libProblem(path);

    bool loadResult = libProblem.load();
    if (!loadResult)
    {
        QString message;
        message.sprintf("Couldn't load problem library: '%s'", path);
        statusBar()->showMessage(message);
        return false;
    }

    m_ptrProblemBrocker = reinterpret_cast<IBrocker *>(libProblem.resolve("getBrocker"));
    if (!m_ptrProblemBrocker)
    {
        QString message;
        message.sprintf("Couldn't resolve 'getBrocker' function from library: '%s'", path);
        statusBar()->showMessage(path);
        return false;
    }

    m_ptrProblem = reinterpret_cast<IProblem *>(m_ptrProblemBrocker->getInterfaceImpl(IBrocker::PROBLEM));
    if (!m_ptrProblem)
    {
        QString message;
        message.sprintf("Couldn't get 'IProblem' implementation from library: '%s'", path);
        statusBar()->showMessage(path);
        return false;
    }

    return true;
}

void MainWindow::solve()
{
}
