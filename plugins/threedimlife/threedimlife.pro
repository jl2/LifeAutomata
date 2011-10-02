TEMPLATE      = lib
CONFIG       += plugin

QT += opengl

CONFIG += debug

HEADERS       = threedimlife.h threedimlifeconfig.h
SOURCES       = threedimlife.cpp threedimlifeconfig.cpp

DESTDIR       = ../../bin/plugins

INCLUDEPATH   += ../../src
