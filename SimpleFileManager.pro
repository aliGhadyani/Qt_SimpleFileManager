#-------------------------------------------------
#
# Project created by QtCreator 2022-01-08T14:00:08
#
#-------------------------------------------------

QT          += core gui
CONFIG      += std=c++1z

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      =  SimpleFileManager
TEMPLATE    =  app

LIBS        += Advapi32.lib\

SOURCES     += main.cpp\
                mainwindow.cpp \
                progress.cpp \
                progressthread.cpp

HEADERS     += mainwindow.h \
                delegateprivate.h \
                progress.h \
                progressthread.h

FORMS       += mainwindow.ui \
                progress.ui

RESOURCES   += resources.qrc
