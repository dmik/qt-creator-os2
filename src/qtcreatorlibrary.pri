include(../qtcreator.pri)

# use precompiled header for libraries by default
isEmpty(PRECOMPILED_HEADER):PRECOMPILED_HEADER = $$PWD/shared/qtcreator_pch.h

win32|os2 {
    DLLDESTDIR = $$IDE_APP_PATH
}

DESTDIR = $$IDE_LIBRARY_PATH

include(rpath.pri)

TARGET = $$qtLibraryName($$TARGET)
os2:!isEmpty(TARGET_SHORT): TARGET_SHORT = $$qtLibraryName($$TARGET_SHORT)

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols

!macx {
    win32|os2 {
        dlltarget.path = /bin
        INSTALLS += dlltarget
    } else {
        target.path = /$$IDE_LIBRARY_BASENAME/qtcreator
        INSTALLS += target
    }
}
