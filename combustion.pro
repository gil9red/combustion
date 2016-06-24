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
    busman.cpp \
    linedaystable.cpp \
    verticalschedulerheaderview.cpp \
    linedaystablemodel.cpp \
    linedays_verticalheaderview.cpp \
    linedays_celldelegate.cpp \
    scheduler_celldelegate.cpp \
    scoreinfoboard.cpp \
    schedulertable.cpp \
    scheduler_table_model.cpp

HEADERS  += mainwindow.h \
    parserpuzzlefile.h \
    busman.h \
    linedaystable.h \
    verticalschedulerheaderview.h \
    linedaystablemodel.h \
    linedays_verticalheaderview.h \
    linedays_celldelegate.h \
    scheduler_celldelegate.h \
    scoreinfoboard.h \
    enums.h \
    schedulertable.h \
    scheduler_table_model.h

FORMS    += mainwindow.ui \
    scoreinfoboard.ui

DISTFILES += \
    INFO/given_pussle.txt

RESOURCES += \
    resource.qrc
