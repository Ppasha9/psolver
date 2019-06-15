#-------------------------------------------------
#
# Project created by QtCreator 2019-06-15T19:26:44
#
#-------------------------------------------------

QT       += core gui

TARGET = PSolver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    Interfaces/IBrocker.h \
    Interfaces/ISolver.h \
    Interfaces/IProblem.h \
    Interfaces/SHARED_EXPORT.h

FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=gnu++0x




