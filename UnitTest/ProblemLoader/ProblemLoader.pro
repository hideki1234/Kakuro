#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T00:21:59
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_problemloadertest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase
CONFIG   += c+=11

TEMPLATE = app


SOURCES += tst_problemloadertest.cpp \
    ../../Kakuro/problemdata.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../Kakuro/problemdata.h
