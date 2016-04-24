#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T11:15:58
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_editorboardtest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase c++11

TEMPLATE = app


SOURCES += tst_editorboardtest.cpp \
    ../../Editor/kkrboardmanager.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../Editor/kkrboardmanager.h
