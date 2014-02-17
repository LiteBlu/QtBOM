#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databasemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButCategories_clicked();
    void on_toolButComponents_clicked();
    void on_toolButMain_Add_clicked();

    void on_bbAddCategory_accepted();

    void on_toolButSuppliers_clicked();

    void on_bbAddEditSupplier_accepted();

private:
    Ui::MainWindow *ui;

    DatabaseManager *dbm;

    enum currentView_t {
        mainComponents,
        mainCategories,
        mainSuppliers
    };

    currentView_t currentView;
};

#endif // MAINWINDOW_H
