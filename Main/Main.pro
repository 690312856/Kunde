#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T16:40:15
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console
TARGET = Main
TEMPLATE = app


SOURCES += main.cpp \
    controler.cpp \
    measure.cpp \
    measurestatus.cpp \
    database.cpp

HEADERS  += \
    controler.h \
    measure.h \
    measurestatus.h \
    database.h


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CalculateMethod/release/ -lCalculateMethod
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CalculateMethod/debug/ -lCalculateMethod

INCLUDEPATH += $$PWD/../CalculateMethod
DEPENDPATH += $$PWD/../CalculateMethod

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Communication/release/ -lCommunication
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Communication/debug/ -lCommunication

INCLUDEPATH += $$PWD/../Communication
DEPENDPATH += $$PWD/../Communication

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UI/release/ -lUI
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UI/debug/ -lUI

INCLUDEPATH += $$PWD/../UI
DEPENDPATH += $$PWD/../UI

OTHER_FILES += \
    icon.rc \
    logo.ico

RC_FILE = icon.rc
