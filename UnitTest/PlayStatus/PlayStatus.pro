#-------------------------------------------------
#
# Project created by QtCreator 2016-04-09T01:16:40
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_playstatustest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase

TEMPLATE = app


SOURCES += tst_playstatustest.cpp \
    ../../Kakuro/playstatus.cpp \
    mock_problemdata.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../Kakuro/playstatus.h
