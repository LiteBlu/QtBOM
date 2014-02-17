#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbm = new DatabaseManager(parent);

    dbm->insertCategory("Resistor");
    dbm->insertCategory("Microcontroller");
    dbm->insertCategory("Capacitor");

    dbm->insertComponent("DES00510", "Resistor 2k2 SMD 0603", 1);
    dbm->insertComponent("DES00511", "Capacitor SMD 0603 100n", 3);
    dbm->insertComponent("DES00512", "NXP LPC11C14", 2);

    this->on_toolButComponents_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButCategories_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    currentView = mainCategories;
    ui->lblMain->setText("Categories");
    ui->tableViewComponents->setModel(dbm->modelCategories);
    ui->tableViewComponents->hideColumn(0); // don't show the ID
}

void MainWindow::on_toolButComponents_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    currentView = mainComponents;
    ui->lblMain->setText("Components");
    ui->tableViewComponents->setModel(dbm->modelComponents);
    ui->tableViewComponents->hideColumn(0); // don't show the ID
}

void MainWindow::on_toolButSuppliers_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    currentView = mainSuppliers;
    ui->lblMain->setText("Suppliers");
    ui->tableViewComponents->setModel(dbm->modelSuppliers);
    ui->tableViewComponents->hideColumn(0); // don't show the ID
}

void MainWindow::on_toolButMain_Add_clicked()
{
    if (currentView == mainCategories)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
    else if (currentView == mainSuppliers)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_bbAddCategory_accepted()
{
    dbm->insertCategoryViaModel(ui->leCategoryName->text());
    ui->leCategoryName->clear();

    this->on_toolButCategories_clicked();
}

void MainWindow::on_bbAddEditSupplier_accepted()
{
    struct Suppliers supplier;
    supplier.name = ui->leSupplierName->text();
    supplier.contact = ui->leSupplierContact->text();
    supplier.address = "";
    supplier.tel = ui->leSupplierTel->text();
    supplier.fax = ui->leSupplierFax->text();
    supplier.email = ui->leSupplierEmail->text();

    ui->leSupplierName->clear();
    ui->leSupplierContact->clear();
    ui->leSupplierTel->clear();
    ui->leSupplierFax->clear();
    ui->leSupplierEmail->clear();

    dbm->insertSupplierViaModel(supplier);

    this->on_toolButSuppliers_clicked();
}
