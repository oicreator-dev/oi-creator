include(../../qtcreatorplugin.pri)

DEFINES += \
    LMAKEPROJECTMANAGER_LIBRARY

HEADERS += \
    lmakeprojectmanagerplugin.h \
    lmakeprojectmanager.h \
    lmakeapi.h \
    configreader.h \
    core.h \
    data.h \
    error.h \
    makefilewritter.h \
    stringtools.h

SOURCES += \
    lmakeprojectmanagerplugin.cpp \
    lmakeprojectmanager.cpp\
    lmakeapi.cpp \
    core.cpp \
    configreader.cpp \
    data.cpp \
    makefilewritter.cpp \
    stringtools.cpp \
    error.cpp

