#-------------------------------------------------
#
# Project created by QtCreator 2012-02-16T11:17:34
#
#-------------------------------------------------

QT       += core gui

TARGET = syswiregpio
TEMPLATE = app
#QMAKE_CXXFLAGS -= -m64
QMAKE_CXXFLAGS += -m32

SOURCES += main.cpp\
    gpiowidget.cpp \
    layout_widg.cpp \
    qtsvgslideswitch.cpp \
    qtsvgpixmapcache.cpp \
    qtsvgtoggleswitch.cpp

HEADERS  += \
    gpiowidget.h \
    center.h \
    layout_widg.h \
    qtsvgslideswitch.h \
    qtsvgpixmapcache.h \
    qtsvgtoggleswitch.h

RESOURCES += \
    graphics.qrc

QT += svg
