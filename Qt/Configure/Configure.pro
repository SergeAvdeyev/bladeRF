TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET += ../../bin/Configure.exe

SOURCES += \
    main.cpp

INCLUDEPATH += "C:/Program Files/bladeRF/include"
#INCLUDEPATH += C:/Qt/Qt5.4.1/Tools/mingw491_32/i686-w64-mingw32/include/c++

LIBS += "C:/Program Files/bladeRF/x86/bladeRF.lib"
