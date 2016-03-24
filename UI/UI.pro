#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T16:42:04
#
#-------------------------------------------------

QT       += widgets

TARGET = UI
TEMPLATE = lib

DEFINES += UI_LIBRARY

SOURCES += \
    widget.cpp \
    inputedit.cpp \
    pointplan.cpp

HEADERS +=\
    widget.h \
    global.h \
    inputedit.h \
    pointplan.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    widget.ui

RESOURCES += \
    resources.qrc
