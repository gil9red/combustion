#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T00:24:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = combustion
TEMPLATE = app
DESTDIR += ../bin

SOURCES += main.cpp\
        mainwindow.cpp \
    parserpuzzlefile.cpp \
    busman.cpp

HEADERS  += mainwindow.h \
    parserpuzzlefile.h \
    busman.h

FORMS    += mainwindow.ui

DISTFILES += \
    INFO/given_pussle.txt
