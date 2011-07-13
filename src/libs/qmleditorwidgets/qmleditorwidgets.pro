TEMPLATE = lib
CONFIG+=dll
TARGET = QmlEditorWidgets
os2:TARGET_SHORT = QmlEdW

DEFINES += QWEAKPOINTER_ENABLE_ARROW

unix:QMAKE_CXXFLAGS_DEBUG += -O3

include(../../qtcreatorlibrary.pri)
include(../qmljs/qmljs.pri)
include(qmleditorwidgets-lib.pri)

