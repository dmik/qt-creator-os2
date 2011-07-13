TEMPLATE = lib
CONFIG += dll
TARGET = LanguageUtils
os2:TARGET_SHORT = LngUtil
DEFINES += QT_CREATOR

unix:QMAKE_CXXFLAGS_DEBUG += -O3

include(../../qtcreatorlibrary.pri)
include(languageutils-lib.pri)
include(../utils/utils.pri)
