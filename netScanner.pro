#-------------------------------------------------
#
# Project created by QtCreator 2013-01-27T18:56:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qfractal
TEMPLATE = app

@
QT += network
@

QT += printsupport

SOURCES += main.cpp\
        mainwindow.cpp \
    interface.cpp \
    os_specifier.cpp \
    network_manager.cpp \
    addressrange.cpp \
    settings.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    interface.h \
    os_specifier.h \
    network_manager.h \
    addressrange.h \
    settings.h \
    about.h

FORMS    += mainwindow.ui \
    settings.ui \
    about.ui
