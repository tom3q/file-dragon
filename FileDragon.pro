#-------------------------------------------------
#
# Project created by QtCreator 2011-05-17T16:55:41
#
#-------------------------------------------------

QT       += core gui

TARGET = FileDragon
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treemapwidget.cpp \
    abstractnode.cpp \
    directorynode.cpp \
    filenode.cpp \
    filetree.cpp \
    filefilter.cpp \
    cellrenderer.cpp

HEADERS  += mainwindow.h \
    treemapwidget.h \
    abstractnode.h \
    directorynode.h \
    filenode.h \
    filetree.h \
    filefilter.h \
    cellrenderer.h

FORMS    += mainwindow.ui
