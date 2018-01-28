#-------------------------------------------------
#
# Project created by QtCreator 2012-07-11T20:58:13
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LanMessenger
TEMPLATE = app

CONFIG += release
QMAKE_CXXFLAGS += -std=c++14
DESTDIR += ../build/LanMessenger

SUBDIRS += \
    LanMessenger.pro

FORMS += \
    dialogabout.ui \
    dialognickname.ui \
    dialogroom.ui \
    mainwindow.ui \
    optionswindow.ui \
    pmwindow.ui \
    roomwindow.ui

HEADERS += \
    dialogabout.h \
    dialognickname.h \
    dialogroom.h \
    historysaver.hpp \
    mainwindow.h \
    messenger.h \
    options.hpp \
    optionswindow.hpp \
    peer.h \
    pmwindow.h \
    roomwindow.h

SOURCES += \
    dialogabout.cpp \
    dialognickname.cpp \
    dialogroom.cpp \
    historysaver.cpp \
    main.cpp \
    mainwindow.cpp \
    messenger.cpp \
    options.cpp \
    optionswindow.cpp \
    peer.cpp \
    pmwindow.cpp \
    roomwindow.cpp

