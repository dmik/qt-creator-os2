TEMPLATE = lib
TARGET = HelloWorld
os2:TARGET_SHORT = HloWrld

include(../../qtcreatorplugin.pri)
include(../../plugins/coreplugin/coreplugin.pri)

HEADERS += helloworldplugin.h \
    helloworldwindow.h

SOURCES += helloworldplugin.cpp \
    helloworldwindow.cpp
