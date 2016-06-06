#-------------------------------------------------
#
# Project created by QtCreator 2016-05-26T12:54:57
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = ../bin/srd_gui
TEMPLATE = app

INCLUDEPATH += /home/savdeev/Projects/qt_components/qcustomplot \
               /home/savdeev/Projects/bladeRF/bladeRF_orig/host/libraries/libbladeRF/include \
               /home/savdeev/Projects/bladeRF/Qt/DSP/Butt_filt/libs
#                /home/savdeev/Projects/dsp_libs/DSPFilters/shared/DSPFilters/include
INCLUDEPATH += /usr/local/include/liquid


SOURCES += main.cpp\
        mainwindow.cpp \
        ../../../../qt_components/qcustomplot/qcustomplot.cpp \
        bladerf_device.cpp \
    rx_thread.cpp \
    fft.cpp \
    ../../DSP/Butt_filt/libs/butt_filter.cpp \
    tx_thread.cpp
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Bessel.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Biquad.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Butterworth.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Cascade.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/ChebyshevI.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/ChebyshevII.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Custom.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Design.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Documentation.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Elliptic.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Filter.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Legendre.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/Param.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/PoleFilter.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/RBJ.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/RootFinder.cpp \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/source/State.cpp \
#    iir_filter.cpp

HEADERS  += mainwindow.h \
        ../../../../qt_components/qcustomplot/qcustomplot.h \
        bladerf_device.h \
        ../../../bladeRF_orig/host/libraries/libbladeRF/include/libbladeRF.h \
    rx_thread.h \
    fft.h \
    ../../DSP/Butt_filt/libs/butt_filter.h \
    tx_thread.h
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Bessel.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Biquad.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Butterworth.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Cascade.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/ChebyshevI.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/ChebyshevII.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Common.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Custom.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Design.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Dsp.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Elliptic.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Filter.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Layout.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Legendre.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/MathSupplement.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Params.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/PoleFilter.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/RBJ.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/RootFinder.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/SmoothedFilter.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/State.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Types.h \
#    ../../dsp_libs/DSPFilters/shared/DSPFilters/include/DspFilters/Utilities.h \
#    iir_filter.h

FORMS    += mainwindow.ui

LIBS += -lbladeRF
LIBS += -lliquid
