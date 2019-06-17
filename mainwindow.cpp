#include <QLibrary>
#include <QFileDialog>
#include <QInputDialog>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

#include <QDeclarativeView>
#include <QDeclarativeItem>

#include "Interfaces/IBrocker.h"
#include "Interfaces/ISolver.h"
#include "Interfaces/IProblem.h"
#include "Interfaces/ILog.h"
#include "Interfaces/Vector.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qsqlconnectiondialog.h"
#include "solverparamsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_ptrProblem(NULL), m_ptrSolver(NULL)
{
    ui->setupUi(this);

    QObject::connect(ui->solveButton, SIGNAL(clicked()), this, SLOT(onSolve()));
    QObject::connect(ui->actionAdd_solver, SIGNAL(triggered()), this, SLOT(onAddSolver()));
    QObject::connect(ui->actionChoose_problem, SIGNAL(triggered()), this, SLOT(onChooseProblem()));

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ILog::init("PSolver.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addConnection()
{
    QSqlConnectionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    QSqlError err = addConnection(dialog.driverName(), dialog.databaseName(), dialog.hostName(),
                                  dialog.userName(), dialog.password(), dialog.port());
    if (err.type() != QSqlError::NoError)
    {
        QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                             "opening the connection: ") + err.text());
    }
}

QSqlError MainWindow::addConnection(const QString &driver, const QString &dbName, const QString &host,
                                    const QString &user, const QString &passwd, int port)
{
    static int cCount = 0;

    QSqlError err;
    m_db = QSqlDatabase::addDatabase(driver, QString("PSolver%1").arg(++cCount));
    m_db.setDatabaseName(dbName);
    m_db.setHostName(host);
    m_db.setPort(port);
    if (!m_db.open(user, passwd))
    {
        err = m_db.lastError();
        m_db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("PSolver%1").arg(cCount));
    }

    selectAll();
    return err;
}

void MainWindow::onSolve()
{
    if (!m_ptrProblem)
    {
        statusBar()->showMessage("No problem selected!");
        return;
    }

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

    if (!selectionModel->hasSelection())
    {
        statusBar()->showMessage("No solver selected!");
        return;
    }

    const int pathColumn = 2;
    QModelIndexList indexList = selectionModel->selectedRows(pathColumn);
    QVariant data = indexList.front().data();
    QString path = data.toString();

    if (!loadSolver(path))
        return;

    // Show parameters dialogue
    QUrl url;
    m_ptrSolver->getQml(url);
    m_ptrSolver->setProblem(m_ptrProblem);

    SolverParamsDialog paramDialog(this);
    QDeclarativeView *&ui = paramDialog.getSolverParamsView();

    QFileInfo info = QFileInfo(path);
    QString dirName = info.path();
    QString absoluteQmlPath = dirName + "/" + url.toString();
    url = QUrl(absoluteQmlPath);

    ui->setSource(url);
    //ui->setSource(QUrl("qrc:/testqml.qml"));

    ui->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    if (paramDialog.exec() != QDialog::Accepted)
        return;

    // Find all text fields
    QString params;
    QObject *root = ui->rootObject();
    for (auto it = root->children().begin(); it != root->children().end(); it++)
    {
        QObject *item = *it;

        QVariant prop = item->property("text");
        if (prop.isNull())
            continue;
        params += prop.toString();
        params += "; ";
    }
    m_ptrSolver->setParams(params);

    m_ptrSolver->solve();

    size_t dim;
    m_ptrProblem->getArgsDim(dim);
    double *sol = new double[dim];

    IVector * solution = Vector::createVector(dim, sol);
    m_ptrSolver->getSolution(solution);

    // SOLUTION MUST BE (-0.7;-0.4)

    for (unsigned int i = 0; i < dim; i++)
    {
        double x;
        solution->getCoord(i, x);
        ILog::report(QString::number(x).toStdString().c_str());
    }

    delete [] sol;
    // Show result window
}

void MainWindow::onChooseProblem()
{
    QString path = QFileDialog::getOpenFileName(0, "Choose problem file", QString(), "*.dll");
    if (path.isEmpty())
        return;
    loadProblem(path);
}

void MainWindow::execQuery(const QString &query)
{
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    model->setQuery(QSqlQuery(query, m_db));
    ui->tableView->setModel(model);

    if (model->lastError().type() != QSqlError::NoError)
        statusBar()->showMessage(model->lastError().text());
    else if (model->query().isSelect())
        statusBar()->showMessage(tr("Query OK."));
    else
        statusBar()->showMessage(tr("Query OK, number of affected rows: %1").arg(
                                 model->query().numRowsAffected()));
}

void MainWindow::selectAll()
{
    execQuery("SELECT * FROM solvers");
}

void MainWindow::onAddSolver()
{
    QString path = QFileDialog::getOpenFileName(0, "Choose solver file", QString(), "*.dll");
    if (path.isEmpty())
        return;

    QString name;
    while (name.isEmpty())
        name = QInputDialog::getText(0, "Input solver name", "Name");

    // Executing the query
    QString query = "INSERT INTO solvers (name, path) VALUES (\'" + name + "\', \'" + path + "\');";
    execQuery(query);
    selectAll();
}

bool MainWindow::loadSolver(const QString &path)
{
    statusBar()->showMessage(QString("Loading solver from " + path + "..."));

    QLibrary libSolver(path);

    bool loadResult = libSolver.load();
    if (!loadResult)
    {
        statusBar()->showMessage(QString("Couldn't load solver library: ") + path);
        return false;
    }

    GetBrockerFunc solverBrockerFunc = reinterpret_cast<GetBrockerFunc>(libSolver.resolve("getBrocker"));
    if (!solverBrockerFunc)
    {
        statusBar()->showMessage(QString("Couldn't resolve 'getBrocker' function from library: ") + path);
        return false;
    }

    IBrocker * ptrSolverBrocker = reinterpret_cast<IBrocker *>(solverBrockerFunc());
    void * interfaceImpl = NULL;
    if (ptrSolverBrocker->canCastTo(IBrocker::SOLVER))
    {
        interfaceImpl = ptrSolverBrocker->getInterfaceImpl(IBrocker::SOLVER);
    }
    else
    {
        statusBar()->showMessage(QString("Couldn't get 'ISolver' implementation from library: ") + path);
        return false;
    }

    m_ptrSolver = reinterpret_cast<ISolver *>(interfaceImpl);
    if (!m_ptrSolver)
    {
        statusBar()->showMessage(QString("Couldn't get 'ISolver' implementation from library: ") + path);
        return false;
    }

    return true;
}

bool MainWindow::loadProblem(const QString &path)
{
    statusBar()->showMessage(QString("Loading problem from " + path + "..."));

    QLibrary libProblem(path);

    bool loadResult = libProblem.load();
    if (!loadResult)
    {
        statusBar()->showMessage(QString("Couldn't load problem library: ") + path);
        return false;
    }

    GetBrockerFunc problemBrockerFunc = reinterpret_cast<GetBrockerFunc>(libProblem.resolve("getBrocker"));
    if (!problemBrockerFunc)
    {
        statusBar()->showMessage(QString("Couldn't resolve 'getBrocker' function from library: ") + path);
        return false;
    }

    IBrocker * ptrProblemBrocker = reinterpret_cast<IBrocker *>(problemBrockerFunc());
    void * interfaceImpl = NULL;
    if (ptrProblemBrocker->canCastTo(IBrocker::PROBLEM))
    {
        interfaceImpl = ptrProblemBrocker->getInterfaceImpl(IBrocker::PROBLEM);
    }
    else
    {
        statusBar()->showMessage(QString("Couldn't get 'IProblem' implementation from library: ") + path);
        return false;
    }

    m_ptrProblem = reinterpret_cast<IProblem *>(interfaceImpl);
    if (!m_ptrProblem)
    {
        statusBar()->showMessage(QString("Couldn't get 'IProblem' implementation from library: ") + path);
        return false;
    }

    statusBar()->showMessage(QString("Problem successfully loaded."));

    double res;
    ErrorEnum err = (ErrorEnum)m_ptrProblem->goalFunction(NULL, NULL, res);

    return true;
}
