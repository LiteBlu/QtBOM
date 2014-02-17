#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include <QLineEdit>

struct Components {
    QString description;
    QString stockCode;
    int category;
};

struct Suppliers {
    QString name;
    QString tel;
    QString fax;
    QString email;
    QString address;
    QString contact;
};

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);

    QSqlRelationalTableModel *modelComponents;
    QSqlTableModel *modelSuppliers;
    QSqlTableModel *modelCategories;
    QSqlTableModel *modelKits;

    QSqlError lastError();
    void submitCategoryName();

    int insertComponent(QString stockCode, QString desc, int category);

    int insertCategory(QString group);
    int insertCategoryViaModel(QString groupName);

    int insertSupplierViaModel(struct Suppliers supplier);

private:
    bool openDB();
    bool deleteDB();

    // Models
    void setupComponentsModel();
    void setupCategoriesModel();
    void setupSuppliersModel();

    // Tables
    bool createComponentsTable();
    bool createCategoriesTable();
    bool createSuppliersTable();

    QSqlDatabase db;

signals:

public slots:

};

#endif // DATABASEMANAGER_H
