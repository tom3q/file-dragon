#-------------------------------------------------
#
# Project created by QtCreator 2011-05-17T16:55:41
#
#-------------------------------------------------

QT       += core gui opengl testlib

TARGET = TreemapWidgetTest
TEMPLATE = app

SOURCES +=	treemapwidgettest.cpp \
			../treemapwidget.cpp \
			../detectionnode.cpp \
			../filetree.cpp \
			../filefilter.cpp \
			../defaultrenderer.cpp \
			../filenode.cpp \
			../directorynode.cpp

HEADERS +=	../treemapwidget.h \
			../detectionnode.h \
			../abstractnode.h \
			../directorynode.h \
			../filenode.h \
			../filetree.h \
			../filefilter.h \
			../abstractrenderer.h \
			../defaultrenderer.h
			