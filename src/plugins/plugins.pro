include(../../qtcreator.pri)

TEMPLATE  = subdirs

SUBDIRS   = \
    clangtools \
    coreplugin \
    texteditor \
    cppeditor \
    diffeditor \
    bookmarks \
    projectexplorer \
    cpptools \
    qtsupport \
    debugger \
    cpaster \
    autotoolsprojectmanager \
    fakevim \
    emacskeys \
    genericprojectmanager \
    pythoneditor \
    classview \
    tasklist \
    macros \
    remotelinux \
    valgrind \
    todo \
    beautifier \
    silversearcher \
    lmakeprojectmanager

!isEmpty(NoRemoveQmakeProjectManager) {
    SUBDIRS+=qmakeprojectmanager
}


qtHaveModule(serialport) {
    SUBDIRS += serialterminal
} else {
    warning("SerialTerminal plugin has been disabled since the Qt SerialPort module is not available.")
}

qtHaveModule(quick) {
    #SUBDIRS += qmlprofiler
} else {
    warning("QmlProfiler plugin has been disabled since the Qt Quick module is not available.")
}

qtHaveModule(help) {
    #SUBDIRS += help
} else {
    warning("Help plugin has been disabled since the Qt Help module is not available.")
}

qtHaveModule(designercomponents_private) {
    #SUBDIRS += designer
} else {
    warning("Qt Widget Designer plugin has been disabled since the Qt Designer module is not available.")
}

DO_NOT_BUILD_QMLDESIGNER = $$(DO_NOT_BUILD_QMLDESIGNER)
isEmpty(DO_NOT_BUILD_QMLDESIGNER):qtHaveModule(quick-private) {
    exists($$[QT_INSTALL_QML]/QtQuick/Controls/qmldir) {
       #SUBDIRS += qmldesigner
    } else {
        warning("QmlDesigner plugin has been disabled since Qt Quick Controls 1 are not installed.")
    }
} else {
    !qtHaveModule(quick-private) {
        warning("QmlDesigner plugin has been disabled since the Qt Quick module is not available.")
    } else {
        warning("QmlDesigner plugin has been disabled since DO_NOT_BUILD_QMLDESIGNER is set.")
    }
}


isEmpty(QBS_INSTALL_DIR): QBS_INSTALL_DIR = $$(QBS_INSTALL_DIR)
exists(../shared/qbs/qbs.pro)|!isEmpty(QBS_INSTALL_DIR): \
    #SUBDIRS += \
    #    qbsprojectmanager

SUBDIRS += \
    clangcodemodel

QTC_NO_CLANG_LIBTOOLING=$$(QTC_NO_CLANG_LIBTOOLING)
isEmpty(QTC_NO_CLANG_LIBTOOLING) {
    SUBDIRS += clangrefactoring
    SUBDIRS += clangpchmanager
} else {
    warning("Not building the clang refactoring plugin and the pch manager plugin.")
}

isEmpty(IDE_PACKAGE_MODE) {
    SUBDIRS += \
        helloworld
}

for(p, SUBDIRS) {
    QTC_PLUGIN_DEPENDS =
    include($$p/$${p}_dependencies.pri)
    pv = $${p}.depends
    $$pv = $$QTC_PLUGIN_DEPENDS
}

linux-* {
     SUBDIRS += debugger/ptracepreload.pro
}


