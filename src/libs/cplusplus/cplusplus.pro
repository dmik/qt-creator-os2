TEMPLATE = lib
CONFIG+=dll
TARGET = CPlusPlus
os2:TARGET_SHORT = CPlusPl

DEFINES += NDEBUG
unix:QMAKE_CXXFLAGS_DEBUG += -O2

include(../../qtcreatorlibrary.pri)
include(cplusplus-lib.pri)
include(../languageutils/languageutils.pri)
