TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += build/include include
DEPENDPATH +=

SOURCES += \
    src/LTE-Tracker.cpp \
    src/dsp.cpp \
    src/capbuf.cpp \
    src/common.cpp \
    src/constants.cpp \
    src/display_thread.cpp \
    src/from_osmocom.cpp \
    src/itpp_ext.cpp \
    src/lte_lib.cpp \
    src/macros.cpp \
    src/producer_thread.cpp \
    src/searcher_thread.cpp \
    src/searcher.cpp \
    src/tracker_thread.cpp

HEADERS += \
    build/include/common.h \
    include/LTE-Tracker.h \
    include/capbuf.h \
    include/constants.h \
    include/dsp.h \
    include/filter_coef.h \
    include/itpp_ext.h \
    include/lte_lib.h \
    include/macros.h \
    include/searcher.h

LIBS += -lbladeRF -litpp -lcurses -lboost_system  -lboost_thread -lrtlsdr
#DEFINES += -DHAVE_BLADERF=1
