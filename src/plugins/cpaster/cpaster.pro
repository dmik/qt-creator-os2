QT += network
TEMPLATE = lib
TARGET = CodePaster
os2:TARGET_SHORT = CodPstr
include(../../qtcreatorplugin.pri)
include(cpaster_dependencies.pri)
HEADERS += cpasterplugin.h \
    settingspage.h \
    protocol.h \
    codepasterprotocol.h \
    pasteview.h \
    cpasterconstants.h \
    codepastersettings.h \
    pastebindotcomprotocol.h \
    pastebindotcomsettings.h \
    pastebindotcaprotocol.h \
    settings.h \
    pasteselectdialog.h \
    columnindicatortextedit.h \
    fileshareprotocol.h \
    fileshareprotocolsettingspage.h
SOURCES += cpasterplugin.cpp \
    settingspage.cpp \
    protocol.cpp \
    codepasterprotocol.cpp \
    pasteview.cpp \
    codepastersettings.cpp \
    pastebindotcomprotocol.cpp \
    pastebindotcomsettings.cpp \
    pastebindotcaprotocol.cpp \
    settings.cpp \
    pasteselectdialog.cpp \
    columnindicatortextedit.cpp \
    fileshareprotocol.cpp \
    fileshareprotocolsettingspage.cpp
FORMS += settingspage.ui \
    pasteselect.ui \
    pasteview.ui \
    pastebindotcomsettings.ui \
    fileshareprotocolsettingswidget.ui
include(../../shared/cpaster/cpaster.pri)
