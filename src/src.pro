
TEMPLATE = app

TARGET = qlife
DEPENDPATH += .
INCLUDEPATH += .
QT += opengl

;; CONFIG += debug

DESTDIR       = ../bin

HEADERS += lifeplugin.h lifewindow.h lifewidget.h

SOURCES += main.cpp lifewindow.cpp lifewidget.cpp

RESOURCES += qlife.qrc

