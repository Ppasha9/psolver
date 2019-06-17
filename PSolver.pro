#-------------------------------------------------
#
# Project created by QtCreator 2019-06-15T19:26:44
#
#-------------------------------------------------

QT       += core gui declarative
QT       += sql
QT       += qml

TARGET = PSolver
TEMPLATE = app

DEFINES += SHARED_BUILT_AS_STATIC


SOURCES += main.cpp\
        mainwindow.cpp \
    qsqlconnectiondialog.cpp \
    solverparamsdialog.cpp \
    resultdialog.cpp

HEADERS  += mainwindow.h \
    Interfaces/IBrocker.h \
    Interfaces/ISolver.h \
    Interfaces/IProblem.h \
    Interfaces/SHARED_EXPORT.h \
    qsqlconnectiondialog.h \
    solverparamsdialog.h \
    Interfaces/ILog.h \
    Interfaces/IVector.h \
    Interfaces/Vector.h \
    resultdialog.h \
    Interfaces/error.h

FORMS    += mainwindow.ui \
            qsqlconnectiondialog.ui \
            solverparamsdialog.ui \
    resultdialog.ui

LIBS += "$$PWD/ILog.dll" \
        "$$PWD/math.dll"

QMAKE_CXXFLAGS += -std=gnu++0x





























