TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
#CONFIG -= qt

INCLUDEPATH += ../libs

SOURCES += \
    main.cpp \
    ../libs/wave_gen.cpp

#include(deployment.pri)
#qtcAddDeployment()

HEADERS += \
    ../libs/wave_gen.h

