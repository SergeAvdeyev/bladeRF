#-------------------------------------------------
#
# Project created by QtCreator 2016-06-01T13:34:30
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = lpf2
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += ../libs

TEMPLATE = app


SOURCES += main.cpp \
    ../libs/wave_gen.cpp

HEADERS += \
    ../libs/wave_gen.h
