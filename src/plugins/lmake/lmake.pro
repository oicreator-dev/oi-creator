include(../../qtcreatorplugin.pri)

DEFINES += \
    LMAKE_LIBRARY

HEADERS += \
    lmakeplugin.h \
    lmakeapi.h \
    configreader.h \
    core.h \
    data.h \
    error.h \
    makefilewritter.h \
    stringtools.h

SOURCES += \
    lmakeplugin.cpp \
    lmakeapi.cpp \
    core.cpp \
    configreader.cpp \
    data.cpp \
    makefilewritter.cpp \
    stringtools.cpp \
    error.cpp

RESOURCES += \
    rc.qrc

TRANSLATIONS += lmake_zh_CN.ts
