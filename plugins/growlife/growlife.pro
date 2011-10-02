TEMPLATE      = lib
CONFIG       += plugin

QT += opengl

CONFIG += debug

HEADERS       = growlife.h growlifeconfig.h
SOURCES       = growlife.cpp growlifeconfig.cpp

DESTDIR       = ../../bin/plugins

INCLUDEPATH   += ../../src
