TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
#CONFIG -= qt

INCLUDEPATH += ../libs

SOURCES += \
    main.cpp \
    ../libs/wave_gen.cpp \
    ../libs/butt_filter.cpp

HEADERS += \
    ../libs/wave_gen.h \
    ../libs/butt_filter.h

