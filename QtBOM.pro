#-------------------------------------------------
#
# Project created by QtCreator 2013-11-02T16:09:19
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtBOM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    databasemanager.cpp \
    components.cpp \
    suppliers.cpp

HEADERS  += mainwindow.h \
    databasemanager.h \
    components.h \
    suppliers.h

FORMS    += mainwindow.ui

RESOURCES += \
    qtbomresources.qrc
