#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T10:15:28
#
#-------------------------------------------------

QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Telemetry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        communication.cpp \
        proxy.cpp \

HEADERS  += mainwindow.h \
        communication.h \
        proxy.h \


FORMS    += mainwindow.ui
