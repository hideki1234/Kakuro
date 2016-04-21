#-------------------------------------------------
#
# Project created by QtCreator 2016-04-21T03:03:46
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_metadatatest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase
CONFIG   += c++11

TEMPLATE = app


SOURCES += tst_metadatatest.cpp \
    ../../Editor/metadatamanager.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../Editor/metadatamanager.h
