#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T10:16:08
#
#-------------------------------------------------

TARGET = Communication
TEMPLATE = lib

DEFINES += COMMUNICATION_LIBRARY

SOURCES += communication.cpp \
    comcommunication.cpp \
    serialports.cpp \
    qextserial/qextserialport.cpp \
    qextserial/qextserialport_win.cpp \
    wait.cpp

HEADERS += communication.h\
    comcommunication.h \
    serialports.h \
    qextserial/qextserialport.h \
    qextserial/qextserialport_global.h \
    qextserial/qextserialport_p.h \
    wait.h \
    com_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
