#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T13:02:13
#
#-------------------------------------------------

QT       += core gui
CONFIG += C++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jinziqi
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    chessregion.cpp \
    robot.cpp

HEADERS  += widget.h \
    chessregion.h \
    robot.h

FORMS    += widget.ui
