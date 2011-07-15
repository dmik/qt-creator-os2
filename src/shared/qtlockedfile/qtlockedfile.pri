INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/qtlockedfile.h
SOURCES += $$PWD/qtlockedfile.cpp

unix:SOURCES += $$PWD/qtlockedfile_unix.cpp
win32:SOURCES += $$PWD/qtlockedfile_win.cpp
os2:SOURCES += $$PWD/qtlockedfile_unix.cpp

win32:contains(TEMPLATE, lib):contains(CONFIG, shared) {
    DEFINES += QT_QTLOCKEDFILE_EXPORT=__declspec(dllexport)
}


