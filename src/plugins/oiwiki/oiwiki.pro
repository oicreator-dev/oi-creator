include(../../qtcreatorplugin.pri)

QT += webenginewidgets network

INCLUDEPATH *= $$IDE_SOURCE_TREE/src/plugins/oiwiki/yaml-cpp/include

DEFINES += \
    OIWIKI_LIBRARY

HEADERS += \
    yaml-cpp/include/yaml-cpp/contrib/anchordict.h \
    yaml-cpp/include/yaml-cpp/contrib/graphbuilder.h \
    yaml-cpp/include/yaml-cpp/node/detail/bool_type.h \
    yaml-cpp/include/yaml-cpp/node/detail/impl.h \
    yaml-cpp/include/yaml-cpp/node/detail/iterator.h \
    yaml-cpp/include/yaml-cpp/node/detail/iterator_fwd.h \
    yaml-cpp/include/yaml-cpp/node/detail/memory.h \
    yaml-cpp/include/yaml-cpp/node/detail/node.h \
    yaml-cpp/include/yaml-cpp/node/detail/node_data.h \
    yaml-cpp/include/yaml-cpp/node/detail/node_iterator.h \
    yaml-cpp/include/yaml-cpp/node/detail/node_ref.h \
    yaml-cpp/include/yaml-cpp/node/convert.h \
    yaml-cpp/include/yaml-cpp/node/emit.h \
    yaml-cpp/include/yaml-cpp/node/impl.h \
    yaml-cpp/include/yaml-cpp/node/iterator.h \
    yaml-cpp/include/yaml-cpp/node/node.h \
    yaml-cpp/include/yaml-cpp/node/parse.h \
    yaml-cpp/include/yaml-cpp/node/ptr.h \
    yaml-cpp/include/yaml-cpp/node/type.h \
    yaml-cpp/include/yaml-cpp/anchor.h \
    yaml-cpp/include/yaml-cpp/binary.h \
    yaml-cpp/include/yaml-cpp/dll.h \
    yaml-cpp/include/yaml-cpp/emitfromevents.h \
    yaml-cpp/include/yaml-cpp/emitter.h \
    yaml-cpp/include/yaml-cpp/emitterdef.h \
    yaml-cpp/include/yaml-cpp/emittermanip.h \
    yaml-cpp/include/yaml-cpp/emitterstyle.h \
    yaml-cpp/include/yaml-cpp/eventhandler.h \
    yaml-cpp/include/yaml-cpp/exceptions.h \
    yaml-cpp/include/yaml-cpp/mark.h \
    yaml-cpp/include/yaml-cpp/null.h \
    yaml-cpp/include/yaml-cpp/ostream_wrapper.h \
    yaml-cpp/include/yaml-cpp/parser.h \
    yaml-cpp/include/yaml-cpp/stlemitter.h \
    yaml-cpp/include/yaml-cpp/traits.h \
    yaml-cpp/include/yaml-cpp/yaml.h \
    yaml-cpp/src/contrib/graphbuilderadapter.h \
    yaml-cpp/src/collectionstack.h \
    yaml-cpp/src/directives.h \
    yaml-cpp/src/emitterstate.h \
    yaml-cpp/src/emitterutils.h \
    yaml-cpp/src/exp.h \
    yaml-cpp/src/indentation.h \
    yaml-cpp/src/nodebuilder.h \
    yaml-cpp/src/nodeevents.h \
    yaml-cpp/src/ptr_vector.h \
    yaml-cpp/src/regex_yaml.h \
    yaml-cpp/src/regeximpl.h \
    yaml-cpp/src/scanner.h \
    yaml-cpp/src/scanscalar.h \
    yaml-cpp/src/scantag.h \
    yaml-cpp/src/setting.h \
    yaml-cpp/src/singledocparser.h \
    yaml-cpp/src/stream.h \
    yaml-cpp/src/streamcharsource.h \
    yaml-cpp/src/stringsource.h \
    yaml-cpp/src/tag.h \
    yaml-cpp/src/token.h \
    oiwikiplugin.h \
    wikimode.h

SOURCES += \
    yaml-cpp/src/contrib/graphbuilder.cpp \
    yaml-cpp/src/contrib/graphbuilderadapter.cpp \
    yaml-cpp/src/binary.cpp \
    yaml-cpp/src/convert.cpp \
    yaml-cpp/src/directives.cpp \
    yaml-cpp/src/emit.cpp \
    yaml-cpp/src/emitfromevents.cpp \
    yaml-cpp/src/emitter.cpp \
    yaml-cpp/src/emitterstate.cpp \
    yaml-cpp/src/emitterutils.cpp \
    yaml-cpp/src/exceptions.cpp \
    yaml-cpp/src/exp.cpp \
    yaml-cpp/src/memory.cpp \
    yaml-cpp/src/node.cpp \
    yaml-cpp/src/node_data.cpp \
    yaml-cpp/src/nodebuilder.cpp \
    yaml-cpp/src/nodeevents.cpp \
    yaml-cpp/src/null.cpp \
    yaml-cpp/src/ostream_wrapper.cpp \
    yaml-cpp/src/parse.cpp \
    yaml-cpp/src/parser.cpp \
    yaml-cpp/src/regex_yaml.cpp \
    yaml-cpp/src/scanner.cpp \
    yaml-cpp/src/scanscalar.cpp \
    yaml-cpp/src/scantag.cpp \
    yaml-cpp/src/scantoken.cpp \
    yaml-cpp/src/simplekey.cpp \
    yaml-cpp/src/singledocparser.cpp \
    yaml-cpp/src/stream.cpp \
    yaml-cpp/src/tag.cpp \
    oiwikiplugin.cpp \
    wikimode.cpp

DISTFILES += \
    yaml-cpp/src/contrib/yaml-cpp.natvis \
    yaml-cpp/src/contrib/yaml-cpp.natvis.md

RESOURCES += \
    oiwiki.qrc

