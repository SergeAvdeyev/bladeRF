#-------------------------------------------------
#
# Project created by QtCreator 2016-05-26T12:54:57
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CustomPlot
TEMPLATE = app

INCLUDEPATH += /home/savdeev/Projects/qt_components/qcustomplot \
                /home/savdeev/Projects/bladeRF/host/libraries/libbladeRF/include


SOURCES += main.cpp\
        mainwindow.cpp \
        ../qt_components/qcustomplot/qcustomplot.cpp \
        bladerf_device.cpp \
    test_thread.cpp

HEADERS  += mainwindow.h \
        ../qt_components/qcustomplot/qcustomplot.h \
        bladerf_device.h \
    ../bladeRF/host/libraries/libbladeRF/include/libbladeRF.h \
    test_thread.h

FORMS    += mainwindow.ui

LIBS += -lbladeRF
