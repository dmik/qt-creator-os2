TEMPLATE = lib
TARGET = ResourceEditor
os2:TARGET_SHORT = ResEdit

qtAddLibrary(QtDesigner)

include(../../qtcreatorplugin.pri)
include(../../libs/utils/utils.pri)
include(../../plugins/coreplugin/coreplugin.pri)
include(../../shared/qrceditor/qrceditor.pri)

INCLUDEPATH += $$PWD/../../tools/utils

HEADERS += resourceeditorfactory.h \
resourceeditorplugin.h \
resourcewizard.h \
resourceeditorw.h \
resourceeditorconstants.h

SOURCES +=resourceeditorfactory.cpp \
resourceeditorplugin.cpp \
resourcewizard.cpp \
resourceeditorw.cpp

RESOURCES += resourceeditor.qrc
