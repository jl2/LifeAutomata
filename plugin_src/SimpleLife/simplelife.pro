TEMPLATE      = lib
CONFIG       += plugin

QT += opengl

HEADERS       = ../../lifeplugin.h simplelife.h simplelifeconfig.h
SOURCES       = simplelife.cpp simplelifeconfig.cpp

DESTDIR       = ../../plugins

INCLUDEPATH   += ../../
