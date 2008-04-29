TEMPLATE = app
CONFIG += qt warn_on thread debug
UI_DIR = tmp
MOC_DIR = tmp
OBJECTS_DIR = tmp
VVERSION = $$[QT_VERSION]
INCLUDEPATH += /usr/include/qwtplot3d-qt4
unix:LIBS += -lqwtplot3d-qt4
DEPENDPATH = $$INCLUDEPATH
DESTDIR = ../bin

linux-g++:QMAKE_CXXFLAGS += -fno-exceptions

MYVERSION = $$[QMAKE_VERSION]

