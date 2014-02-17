#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include <QObject>

class Suppliers : public QObject
{
    Q_OBJECT
public:
    explicit Suppliers(QObject *parent = 0);

    QString *name;
    QString *tel;
    QString *fax;
    QString *address;
    QString *contact;
    QString *email;

signals:

public slots:

};

#endif // SUPPLIERS_H
