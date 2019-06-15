#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

QT_FORWARD_DECLARE_CLASS(QSqlError)

QT_FORWARD_DECLARE_CLASS(IBrocker)
QT_FORWARD_DECLARE_CLASS(IProblem)
QT_FORWARD_DECLARE_CLASS(ISolver)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addConnection();

private:
    Ui::MainWindow *ui;

    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
                            const QString &user, const QString &passwd, int port = -1);

    bool loadSolver(const QString &path);
    bool loadProblem(const QString &path);

    void solve();

private slots:
    void onSolve();
    void onChooseTask();
    void onAddSolver();

private:
    IBrocker *m_ptrProblemBrocker;
    IBrocker *m_ptrSolverBrocker;

    IProblem *m_ptrProblem;
    ISolver  *m_ptrSolver;
};

#endif // MAINWINDOW_H
