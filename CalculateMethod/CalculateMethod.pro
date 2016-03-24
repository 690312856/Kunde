#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T10:30:42
#
#-------------------------------------------------

QT    += widgets gui multimedia
TARGET = CalculateMethod
TEMPLATE = lib

DEFINES += CAL_LIBRARY

SOURCES += \
    calculatemethod.cpp \
    slicemethod.cpp \
    readdata.cpp

HEADERS += \
    calculatemethod.h \
    slicemethod.h \
    readdata.h \
    cal_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resources.qrc

