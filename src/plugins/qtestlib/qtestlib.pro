TEMPLATE = lib
TARGET   = QTestLibPlugin
os2:TARGET_SHORT = QTstLib
QT      += xml

include(../../qtcreatorplugin.pri)
include(../../plugins/coreplugin/coreplugin.pri)

SOURCES += qtestlibplugin.cpp
HEADERS += qtestlibplugin.h
RESOURCES += qtestlib.qrc

LIBS += -lProjectExplorer \
        -lQuickOpen \
        -lUtils
