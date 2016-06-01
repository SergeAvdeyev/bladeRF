#-------------------------------------------------
#
# Project created by QtCreator 2016-05-25T14:25:30
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = dsp_fft
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    fft.cpp \
    cfft.cpp

HEADERS += \
    fft.h \
    cfft.h
