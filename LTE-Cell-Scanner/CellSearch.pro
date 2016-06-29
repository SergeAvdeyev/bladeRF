TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += build/include include
DEPENDPATH +=

SOURCES += \
    src/CellSearch.cpp \
    src/itpp_ext.cpp \
    src/capbuf.cpp \
    src/searcher.cpp \
    src/constants.cpp \
    src/common.cpp \
    src/lte_lib.cpp \
    src/from_osmocom.cpp

HEADERS +=

LIBS +=  -lrtlsdr -litpp -lcurses -lboost_system  -lboost_thread
