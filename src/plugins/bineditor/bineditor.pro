TEMPLATE = lib
TARGET = BinEditor
os2:TARGET_SHORT = BinEdit
include(../../qtcreatorplugin.pri)
include(bineditor_dependencies.pri)

HEADERS += bineditorplugin.h \
        bineditor.h \
        bineditorconstants.h

SOURCES += bineditorplugin.cpp \
        bineditor.cpp

RESOURCES +=
