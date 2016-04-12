#-------------------------------------------------
#
# Project created by QtCreator 2016-04-12T01:33:38
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_useranswertest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase
CONFIG   += c++11

TEMPLATE = app


SOURCES += tst_useranswertest.cpp \
    mock4useranswer.cpp \
    ../../Kakuro/useranswer.cpp \
    ../../Kakuro/useranswermanager.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../Kakuro/useranswer.h \
    ../../Kakuro/useranswermanager.h
