TEMPLATE = lib
TARGET = QtConcurrent
os2:TARGET_SHORT = QtConcu
DEFINES += BUILD_QTCONCURRENT

include(../../qtcreatorlibrary.pri)

HEADERS += \
    qtconcurrent_global.h \
    multitask.h \
    runextensions.h
