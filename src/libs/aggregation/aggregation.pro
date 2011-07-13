TEMPLATE = lib
TARGET = Aggregation
os2:TARGET_SHORT = Aggrega

include(../../qtcreatorlibrary.pri)

DEFINES += AGGREGATION_LIBRARY

HEADERS = aggregate.h \
    aggregation_global.h

SOURCES = aggregate.cpp

