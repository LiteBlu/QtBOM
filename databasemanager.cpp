#include "databasemanager.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QDir>
#include <QSqlRecord>

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
    this->openDB();
    this->modelComponents = new QSqlRelationalTableModel(parent, db);
    this->modelCategories = new QSqlTableModel(parent, db);
    this->modelSuppliers = new QSqlTableModel(parent, db);

    this->setupComponentsModel();
    this->setupCategoriesModel();
    this->setupSuppliersModel();

    this->createComponentsTable();
    this->createCategoriesTable();
    this->createSuppliersTable();
}

void DatabaseManager::setupComponentsModel()
{
    this->modelComponents->setTable("components");
    this->modelComponents->setEditStrategy(QSqlTableModel::OnFieldChange);
    this->modelComponents->setRelation(this->modelComponents->fieldIndex("category"), QSqlRelation("categories", "id", "name"));
    this->modelComponents->select();
    this->modelComponents->setHeaderData(0, Qt::Horizontal, tr(""));
    this->modelComponents->setHeaderData(1, Qt::Horizontal, tr("Description"));
    this->modelComponents->setHeaderData(2, Qt::Horizontal, tr("Stock Code"));
    this->modelComponents->setHeaderData(3, Qt::Horizontal, tr("Category"));
}

void DatabaseManager::setupCategoriesModel()
{
    this->modelCategories->setTable("categories");
    this->modelCategories->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->modelCategories->select();
    this->modelCategories->setHeaderData(0, Qt::Horizontal, tr(""));
    this->modelCategories->setHeaderData(1, Qt::Horizontal, tr("Category Name"));
}

void DatabaseManager::setupSuppliersModel()
{
    this->modelSuppliers->setTable("suppliers");
    this->modelSuppliers->setEditStrategy(QSqlTableModel::OnFieldChange);
    this->modelSuppliers->select();
    this->modelSuppliers->setHeaderData(0, Qt::Horizontal, tr(""));
    this->modelSuppliers->setHeaderData(1, Qt::Horizontal, tr("Name"));
    this->modelSuppliers->setHeaderData(2, Qt::Horizontal, tr("Contact Person"));
    this->modelSuppliers->setHeaderData(3, Qt::Horizontal, tr("Telephone"));
    this->modelSuppliers->setHeaderData(4, Qt::Horizontal, tr("Fax"));
    this->modelSuppliers->setHeaderData(5, Qt::Horizontal, tr("E-Mail"));
    this->modelSuppliers->setHeaderData(6, Qt::Horizontal, tr("Address"));
}

bool DatabaseManager::openDB()
{
    QDir dir;
    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("QtBOM.db.s3db");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    #else
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("QtBOM");
    // Create directory
    dir.mkpath(path);
    path.append(QDir::separator()).append("QtBOM.db.s3db");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    #endif
    qDebug() << path;

    // Open databasee
    return db.open();
}

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();
}

bool DatabaseManager::deleteDB()
{
    // Close database
    db.close();

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("QtBOM.db.s3db");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else

    // Remove created database binary file
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("QtBOM");
    path.append(QDir::separator()).append("QtBOM.db.s3db");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #endif
}

bool DatabaseManager::createSuppliersTable()
{
    // Create table "suppliers"
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query(db);
        ret = query.exec("create table suppliers "
                  "(id INTEGER PRIMARY KEY, "
                  "name TEXT, "
                  "contact TEXT, "
                  "tel TEXT, "
                  "fax TEXT, "
                  "email TEXT, "
                  "address TEXT)");

    }
    return ret;
}

bool DatabaseManager::createComponentsTable()
{
    // Create table "components"
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query(db);
        ret = query.exec("CREATE TABLE components "
                         "(id INTEGER PRIMARY KEY, "
                         "description TEXT, "
                         "stockCode TEXT) UNIQUE, "
                         "category INTEGER)");
    }
    return ret;
}

bool DatabaseManager::createCategoriesTable()
{
    // Create table "categories"
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query(db);
        ret = query.exec("CREATE TABLE categories "
                         "(id INTEGER PRIMARY KEY, "
                         "name TEXT UNIQUE)");
    }
    return ret;
}

int DatabaseManager::insertComponent(QString stockCode, QString desc, int group)
{
    int newId = -1;
    bool ret = false;

    if (db.isOpen())
        {
        //http://www.sqlite.org/autoinc.html
        // NULL = is the keyword for the autoincrement to generate next value

        QSqlQuery query;
        ret = query.exec(QString("insert into components values(NULL,'%1','%2',%3)")
        .arg(desc).arg(stockCode).arg(group));

        // Get database given autoincrement value
        if (ret)
            {
            // http://www.sqlite.org/c3ref/last_insert_rowid.html
            newId = query.lastInsertId().toInt();
            }

        }
    return newId;
}

int DatabaseManager::insertCategory(QString categoryName)
{
    int newId = -1;
    bool ret = false;

    if (db.isOpen())
        {
        //http://www.sqlite.org/autoinc.html
        // NULL = is the keyword for the autoincrement to generate next value

        QSqlQuery query;
        ret = query.exec(QString("insert into categories values(NULL,'%1')")
        .arg(categoryName));

        // Get database given autoincrement value
        if (ret)
            {
            // http://www.sqlite.org/c3ref/last_insert_rowid.html
            newId = query.lastInsertId().toInt();
            }

        }
    return newId;
}

int DatabaseManager::insertCategoryViaModel(QString groupName)
{
    int retVal;

    QSqlRecord record = this->modelCategories->record();
    record.setValue("name", groupName);

    // -1 means append
    retVal =  this->modelCategories->insertRecord(-1, record);
    this->modelCategories->submitAll();

    return retVal;
}

int DatabaseManager::insertSupplierViaModel(struct Suppliers supplier)
{
    int retVal;

    QSqlRecord record = this->modelSuppliers->record();
    record.setValue("name", supplier.name);
    record.setValue("contact", supplier.contact);
    record.setValue("tel", supplier.tel);
    record.setValue("fax", supplier.fax);
    record.setValue("address", supplier.address);
    record.setValue("email", supplier.email);

    // -1 means append
    retVal =  this->modelSuppliers->insertRecord(-1, record);
    this->modelSuppliers->submitAll();

    return retVal;
}
