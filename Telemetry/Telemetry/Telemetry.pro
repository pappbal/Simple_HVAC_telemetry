#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T10:15:28
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += core



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += qml quick widgets

TARGET = Telemetry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        communication.cpp \
        proxy.cpp \
        application.cpp \
        statehistory.cpp \
        gui.cpp

HEADERS  += mainwindow.h \
        communication.h \
        proxy.h \
        application.h \
        statehistory.h \
        gui.h



FORMS    += mainwindow.ui

CONFIG += c++11

RESOURCES += \
        qml.qrc
