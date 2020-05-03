#-------------------------------------------------
#
# Project created by QtCreator 2020-04-30T16:54:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0
QMAKE_TARGET_COMPANY = KorZubErg
QMAKE_TARGET_PRODUCT = ZVN UDN
QMAKE_TARGET_DESCRIPTION = Eval ZVN UDN
QMAKE_TARGET_COPYRIGHT = Zub, Kordonsky, Erg - 703 MAI

TARGET = ZVN_UDN
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    animation.cpp

HEADERS  += mainwindow.h \
    animation.h

FORMS    += mainwindow.ui

RC_FILE = myico.rc
