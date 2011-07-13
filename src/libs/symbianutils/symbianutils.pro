TEMPLATE = lib
CONFIG+=dll
TARGET = symbianutils
os2:TARGET_SHORT = symbutl
DEFINES += SYMBIANUTILS_BUILD_LIB
include(../../qtcreatorlibrary.pri)
include(../../shared/symbianutils/symbianutils.pri)
