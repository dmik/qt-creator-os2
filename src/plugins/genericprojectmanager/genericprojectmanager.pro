TEMPLATE = lib
TARGET = GenericProjectManager
os2:TARGET_SHORT = GenPMgr
include(../../qtcreatorplugin.pri)
include(genericprojectmanager_dependencies.pri)
HEADERS = genericproject.h \
    genericprojectplugin.h \
    generictarget.h \
    genericprojectmanager.h \
    genericprojectconstants.h \
    genericprojectnodes.h \
    genericprojectwizard.h \
    genericprojectfileseditor.h \
    pkgconfigtool.h \
    genericmakestep.h \
    genericbuildconfiguration.h
SOURCES = genericproject.cpp \
    genericprojectplugin.cpp \
    generictarget.cpp \
    genericprojectmanager.cpp \
    genericprojectnodes.cpp \
    genericprojectwizard.cpp \
    genericprojectfileseditor.cpp \
    pkgconfigtool.cpp \
    genericmakestep.cpp \
    genericbuildconfiguration.cpp
RESOURCES += genericproject.qrc
FORMS += genericmakestep.ui
