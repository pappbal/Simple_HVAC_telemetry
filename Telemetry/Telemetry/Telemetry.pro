#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T10:15:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Telemetry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    data_logger.cpp

HEADERS  += mainwindow.h \
    data_logger.h

FORMS    += mainwindow.ui
