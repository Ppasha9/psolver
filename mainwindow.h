#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlError>

namespace Ui {
    class MainWindow;
}

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

    void loadSolver(const QString &path);
    void loadProblem(const QString &path);

    void solve();

private slots:
    void onSolve();
    void onChooseTask(const QString &path);
    void onAddSolver();
};

#endif // MAINWINDOW_H
