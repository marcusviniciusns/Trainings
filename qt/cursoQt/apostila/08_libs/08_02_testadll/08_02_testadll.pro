#-------------------------------------------------
#
# Project created by QtCreator 2009-10-10T14:53:12
#
#-------------------------------------------------

QT       -= gui

TARGET = 
CONFIG   += console
CONFIG   -= app_bundle

# diret�rios de libs (L mai�sculo):
win32:LIBS +=  -Lc:\cursoQt
unix:LIBS += -L/home/user/cursoQt
# nome das libs ( l min�sculo):
LIBS += -l08_01_Dll

TEMPLATE = app


SOURCES += main.cpp
