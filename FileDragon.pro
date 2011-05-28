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
    defaultrenderer.cpp \
    osoperations.cpp \
    filterdialog.cpp \
    coloringdialog.cpp \
    daterenderer.cpp \
    treemanager.cpp \
    fileinfodialog.cpp \
    fileframe.cpp

HEADERS  += mainwindow.h \
    treemapwidget.h \
    abstractnode.h \
    directorynode.h \
    filenode.h \
    filetree.h \
    filefilter.h \
    defaultrenderer.h \
    osoperations.h \
    filterdialog.h \
    coloringdialog.h \
    daterenderer.h \
    filterdialog.h \
    treemanager.h \
    fileinfodialog.h \
    fileframe.h

FORMS    += mainwindow.ui \
    filterdialog.ui \
    coloringdialog.ui \
    fileinfodialog.ui

RESOURCES	+= FileDragon.qrc
