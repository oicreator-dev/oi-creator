import qbs 1.0
import qbs.FileInfo

Project {
    name: "QmlDesigner"

    QtcDevHeaders { }

    QtcPlugin {
        fileName: FileInfo.fileName(filePath)

        Depends {
            name: "Qt";
            submodules: [
                "core-private", "quickwidgets"
            ]
        }
        Depends { name: "Core" }
        Depends { name: "QmlJS" }
        Depends { name: "QmlEditorWidgets" }
        Depends { name: "TextEditor" }
        Depends { name: "QmlJSEditor" }
        Depends { name: "QmakeProjectManager" }
        Depends { name: "QmlProjectManager" }
        Depends { name: "ProjectExplorer" }
        Depends { name: "LanguageUtils" }
        Depends { name: "QtSupport" }
        Depends { name: "app_version_header" }

        cpp.defines: base.concat("DESIGNER_CORE_LIBRARY")
        cpp.enableExceptions: true
        cpp.includePaths: base.concat([
             ".",
             "designercore",
             "designercore/include",
             "designercore/instances",
             "../../../share/qtcreator/qml/qmlpuppet/interfaces",
             "../../../share/qtcreator/qml/qmlpuppet/container",
             "../../../share/qtcreator/qml/qmlpuppet/commands",
             "../../../share/qtcreator/qml/qmlpuppet/types",
             "components/componentcore",
             "components/debugview",
             "components/importmanager",
             "components/integration",
             "components/propertyeditor",
             "components/formeditor",
             "components/itemlibrary",
             "components/navigator",
             "components/pluginmanager",
             "components/stateseditor",
             "components/texteditor",
             "qmldesignerextension",
             "qmldesignerextension/connectioneditor",
        ])

        Properties {
            condition: qbs.targetOS.contains("unix") && !qbs.targetOS.contains("bsd")
            cpp.dynamicLibraries: base.concat("rt")
        }

        Export {
            Depends { name: "QmlJS" }
            cpp.includePaths: base.concat([
                product.sourceDirectory,
                product.sourceDirectory + "/components/componentcore",
                product.sourceDirectory + "/components/formeditor",
                product.sourceDirectory + "/components/integration",
                product.sourceDirectory + "/designercore",
                product.sourceDirectory + "/designercore/include",
                qtc.export_data_base + "/qml/qmlpuppet/interfaces",
            ])
        }

        Group {
            prefix: "designercore/filemanager/"
            files: [
                "addarraymembervisitor.cpp",
                "addarraymembervisitor.h",
                "addobjectvisitor.cpp",
                "addobjectvisitor.h",
                "addpropertyvisitor.cpp",
                "addpropertyvisitor.h",
                "astobjecttextextractor.cpp",
                "astobjecttextextractor.h",
                "changeimportsvisitor.cpp",
                "changeimportsvisitor.h",
                "changeobjecttypevisitor.cpp",
                "changeobjecttypevisitor.h",
                "changepropertyvisitor.cpp",
                "changepropertyvisitor.h",
                "firstdefinitionfinder.cpp",
                "firstdefinitionfinder.h",
                "moveobjectbeforeobjectvisitor.cpp",
                "moveobjectbeforeobjectvisitor.h",
                "moveobjectvisitor.cpp",
                "moveobjectvisitor.h",
                "objectlengthcalculator.cpp",
                "objectlengthcalculator.h",
                "qmlrefactoring.cpp",
                "qmlrefactoring.h",
                "qmlrewriter.cpp",
                "qmlrewriter.h",
                "removepropertyvisitor.cpp",
                "removepropertyvisitor.h",
                "removeuiobjectmembervisitor.cpp",
                "removeuiobjectmembervisitor.h",
            ]
        }

        Group {
            prefix: "../../../share/qtcreator/qml/qmlpuppet/"
            files: [
                "commands/changeauxiliarycommand.cpp",
                "commands/changeauxiliarycommand.h",
                "commands/changebindingscommand.cpp",
                "commands/changebindingscommand.h",
                "commands/changefileurlcommand.cpp",
                "commands/changefileurlcommand.h",
                "commands/changeidscommand.cpp",
                "commands/changeidscommand.h",
                "commands/changenodesourcecommand.cpp",
                "commands/changenodesourcecommand.h",
                "commands/changestatecommand.cpp",
                "commands/changestatecommand.h",
                "commands/changevaluescommand.cpp",
                "commands/changevaluescommand.h",
                "commands/childrenchangedcommand.cpp",
                "commands/childrenchangedcommand.h",
                "commands/clearscenecommand.cpp",
                "commands/clearscenecommand.h",
                "commands/completecomponentcommand.cpp",
                "commands/completecomponentcommand.h",
                "commands/componentcompletedcommand.cpp",
                "commands/componentcompletedcommand.h",
                "commands/createinstancescommand.cpp",
                "commands/createinstancescommand.h",
                "commands/createscenecommand.cpp",
                "commands/createscenecommand.h",
                "commands/debugoutputcommand.cpp",
                "commands/debugoutputcommand.h",
                "commands/endpuppetcommand.cpp",
                "commands/endpuppetcommand.h",
                "commands/informationchangedcommand.cpp",
                "commands/informationchangedcommand.h",
                "commands/pixmapchangedcommand.cpp",
                "commands/pixmapchangedcommand.h",
                "commands/puppetalivecommand.cpp",
                "commands/puppetalivecommand.h",
                "commands/removeinstancescommand.cpp",
                "commands/removeinstancescommand.h",
                "commands/removepropertiescommand.cpp",
                "commands/removepropertiescommand.h",
                "commands/removesharedmemorycommand.cpp",
                "commands/removesharedmemorycommand.h",
                "commands/reparentinstancescommand.cpp",
                "commands/reparentinstancescommand.h",
                "commands/statepreviewimagechangedcommand.cpp",
                "commands/statepreviewimagechangedcommand.h",
                "commands/synchronizecommand.cpp",
                "commands/synchronizecommand.h",
                "commands/tokencommand.cpp",
                "commands/tokencommand.h",
                "commands/valueschangedcommand.cpp",
                "commands/valueschangedcommand.h",
                "container/addimportcontainer.cpp",
                "container/addimportcontainer.h",
                "container/idcontainer.cpp",
                "container/idcontainer.h",
                "container/imagecontainer.cpp",
                "container/imagecontainer.h",
                "container/informationcontainer.cpp",
                "container/informationcontainer.h",
                "container/instancecontainer.cpp",
                "container/instancecontainer.h",
                "container/mockuptypecontainer.cpp",
                "container/mockuptypecontainer.h",
                "container/propertyabstractcontainer.cpp",
                "container/propertyabstractcontainer.h",
                "container/propertybindingcontainer.cpp",
                "container/propertybindingcontainer.h",
                "container/propertyvaluecontainer.cpp",
                "container/propertyvaluecontainer.h",
                "container/reparentcontainer.cpp",
                "container/reparentcontainer.h",
                "container/sharedmemory.h",
                "interfaces/commondefines.h",
                "interfaces/nodeinstanceclientinterface.h",
                "interfaces/nodeinstanceserverinterface.cpp",
                "interfaces/nodeinstanceserverinterface.h",
                "types/enumeration.cpp",
                "types/enumeration.h",
            ]
        }

        Group {
            name: "SharedMemory (Unix)"
            condition: qbs.targetOS.contains("unix")
            files: [
                "../../../share/qtcreator/qml/qmlpuppet/container/sharedmemory_unix.cpp",
            ]
        }

        Group {
            name: "SharedMemory (Generic)"
            condition: !qbs.targetOS.contains("unix")
            files: [
                "../../../share/qtcreator/qml/qmlpuppet/container/sharedmemory_qt.cpp",
            ]
        }

        Group {
            prefix: "designercore/"
            files: [
                "rewritertransaction.cpp",
                "rewritertransaction.h",
                "exceptions/exception.cpp",
                "exceptions/invalidargumentexception.cpp",
                "exceptions/invalididexception.cpp",
                "exceptions/invalidmetainfoexception.cpp",
                "exceptions/invalidmodelnodeexception.cpp",
                "exceptions/invalidmodelstateexception.cpp",
                "exceptions/invalidpropertyexception.cpp",
                "exceptions/invalidqmlsourceexception.cpp",
                "exceptions/invalidreparentingexception.cpp",
                "exceptions/invalidslideindexexception.cpp",
                "exceptions/notimplementedexception.cpp",
                "exceptions/removebasestateexception.cpp",
                "exceptions/rewritingexception.cpp",
                "include/abstractproperty.h",
                "include/abstractview.h",
                "include/anchorline.h",
                "include/basetexteditmodifier.h",
                "include/bindingproperty.h",
                "include/componenttextmodifier.h",
                "include/customnotifications.h",
                "include/exception.h",
                "include/forwardview.h",
                "include/import.h",
                "include/invalidargumentexception.h",
                "include/invalididexception.h",
                "include/invalidmetainfoexception.h",
                "include/invalidmodelnodeexception.h",
                "include/invalidmodelstateexception.h",
                "include/invalidpropertyexception.h",
                "include/invalidqmlsourceexception.h",
                "include/invalidreparentingexception.h",
                "include/invalidslideindexexception.h",
                "include/itemlibraryinfo.h",
                "include/mathutils.h",
                "include/metainfo.h",
                "include/metainforeader.h",
                "include/model.h",
                "include/modelmerger.h",
                "include/modelnode.h",
                "include/modelnodepositionstorage.h",
                "include/nodeabstractproperty.h",
                "include/nodeinstance.h",
                "include/nodeinstanceview.h",
                "include/nodelistproperty.h",
                "include/nodemetainfo.h",
                "include/nodehints.h",
                "include/nodeproperty.h",
                "include/notimplementedexception.h",
                "include/plaintexteditmodifier.h",
                "include/propertycontainer.h",
                "include/propertynode.h",
                "include/propertyparser.h",
                "include/qmlanchors.h",
                "include/qmlchangeset.h",
                "include/qmlitemnode.h",
                "include/qmlmodelnodefacade.h",
                "include/qmlobjectnode.h",
                "include/qmlstate.h",
                "include/removebasestateexception.h",
                "include/documentmessage.h",
                "include/rewriterview.h",
                "include/rewritingexception.h",
                "include/signalhandlerproperty.h",
                "include/viewmanager.h",
                "include/subcomponentmanager.h",
                "include/textmodifier.h",
                "include/variantproperty.h",
                "include/qmltimelinekeyframes.h",
                "include/qmltimelinemutator.h",
                "instances/nodeinstance.cpp",
                "instances/nodeinstanceserverproxy.cpp",
                "instances/nodeinstanceserverproxy.h",
                "instances/nodeinstanceview.cpp",
                "instances/puppetbuildprogressdialog.cpp",
                "instances/puppetbuildprogressdialog.h",
                "instances/puppetbuildprogressdialog.ui",
                "instances/puppetcreator.cpp",
                "instances/puppetcreator.h",
                "instances/puppetdialog.cpp",
                "instances/puppetdialog.h",
                "instances/puppetdialog.ui",
                "metainfo/itemlibraryinfo.cpp",
                "metainfo/metainfo.cpp",
                "metainfo/metainforeader.cpp",
                "metainfo/nodemetainfo.cpp",
                "metainfo/nodehints.cpp",
                "metainfo/subcomponentmanager.cpp",
                "model/abstractproperty.cpp",
                "model/abstractview.cpp",
                "model/anchorline.cpp",
                "model/basetexteditmodifier.cpp",
                "model/bindingproperty.cpp",
                "model/componenttextmodifier.cpp",
                "model/import.cpp",
                "model/internalbindingproperty.cpp",
                "model/internalbindingproperty.h",
                "model/internalnode.cpp",
                "model/internalnode_p.h",
                "model/internalnodeabstractproperty.cpp",
                "model/internalnodeabstractproperty.h",
                "model/internalnodelistproperty.cpp",
                "model/internalnodelistproperty.h",
                "model/internalnodeproperty.cpp",
                "model/internalnodeproperty.h",
                "model/internalproperty.cpp",
                "model/internalproperty.h",
                "model/internalsignalhandlerproperty.cpp",
                "model/internalsignalhandlerproperty.h",
                "model/internalvariantproperty.cpp",
                "model/internalvariantproperty.h",
                "model/model.cpp",
                "model/model_p.h",
                "model/modelmerger.cpp",
                "model/modelnode.cpp",
                "model/modelnodepositionrecalculator.cpp",
                "model/modelnodepositionrecalculator.h",
                "model/modelnodepositionstorage.cpp",
                "model/modeltotextmerger.cpp",
                "model/modeltotextmerger.h",
                "model/nodeabstractproperty.cpp",
                "model/nodelistproperty.cpp",
                "model/nodeproperty.cpp",
                "model/plaintexteditmodifier.cpp",
                "model/propertycontainer.cpp",
                "model/propertynode.cpp",
                "model/propertyparser.cpp",
                "model/qmlanchors.cpp",
                "model/qmlchangeset.cpp",
                "model/qmlitemnode.cpp",
                "model/qmlmodelnodefacade.cpp",
                "model/qmlobjectnode.cpp",
                "model/qmlstate.cpp",
                "model/qmltextgenerator.cpp",
                "model/qmltextgenerator.h",
                "model/rewriteaction.cpp",
                "model/rewriteaction.h",
                "model/rewriteactioncompressor.cpp",
                "model/rewriteactioncompressor.h",
                "model/documentmessage.cpp",
                "model/rewriterview.cpp",
                "model/signalhandlerproperty.cpp",
                "model/textmodifier.cpp",
                "model/texttomodelmerger.cpp",
                "model/texttomodelmerger.h",
                "model/variantproperty.cpp",
                "model/viewmanager.cpp",
                "model/qmltimelinekeyframes.cpp",
                "model/qmltimelinemutator.cpp",
                "pluginmanager/widgetpluginmanager.cpp",
                "pluginmanager/widgetpluginmanager.h",
                "pluginmanager/widgetpluginpath.cpp",
                "pluginmanager/widgetpluginpath.h",
            ]
        }

        Group {
            prefix: "components/"
            files: [
                "componentcore/abstractaction.cpp",
                "componentcore/abstractaction.h",
                "componentcore/abstractactiongroup.cpp",
                "componentcore/abstractactiongroup.h",
                "componentcore/actioninterface.h",
                "componentcore/addsignalhandlerdialog.cpp",
                "componentcore/addsignalhandlerdialog.h",
                "componentcore/addsignalhandlerdialog.ui",
                "componentcore/componentcore_constants.h",
                "componentcore/crumblebar.cpp",
                "componentcore/crumblebar.h",
                "componentcore/designeractionmanager.cpp",
                "componentcore/designeractionmanager.h",
                "componentcore/designeractionmanagerview.cpp",
                "componentcore/designeractionmanagerview.h",
                "componentcore/findimplementation.cpp",
                "componentcore/findimplementation.h",
                "componentcore/layoutingridlayout.cpp",
                "componentcore/layoutingridlayout.h",
                "componentcore/theme.cpp",
                "componentcore/theme.h",
                "componentcore/modelnodecontextmenu.cpp",
                "componentcore/modelnodecontextmenu.h",
                "componentcore/modelnodecontextmenu_helper.cpp",
                "componentcore/modelnodecontextmenu_helper.h",
                "componentcore/modelnodeoperations.cpp",
                "componentcore/modelnodeoperations.h",
                "componentcore/selectioncontext.cpp",
                "componentcore/selectioncontext.h",
                "componentcore/qmldesignericonprovider.cpp",
                "componentcore/qmldesignericonprovider.h",
                "componentcore/componentcore.qrc",
                "texteditor/texteditorstatusbar.cpp",
                "texteditor/texteditorstatusbar.h",
                "componentcore/changestyleaction.cpp",
                "componentcore/changestyleaction.h",
                "texteditor/texteditorview.cpp",
                "texteditor/texteditorview.h",
                "texteditor/texteditorwidget.cpp",
                "texteditor/texteditorwidget.h",
                "debugview/debugview.cpp",
                "debugview/debugview.h",
                "debugview/debugviewwidget.cpp",
                "debugview/debugviewwidget.h",
                "debugview/debugviewwidget.ui",
                "formeditor/abstractcustomtool.cpp",
                "formeditor/abstractcustomtool.h",
                "formeditor/abstractformeditortool.cpp",
                "formeditor/abstractformeditortool.h",
                "formeditor/anchorindicator.cpp",
                "formeditor/anchorindicator.h",
                "formeditor/anchorindicatorgraphicsitem.cpp",
                "formeditor/anchorindicatorgraphicsitem.h",
                "formeditor/backgroundaction.cpp",
                "formeditor/backgroundaction.h",
                "formeditor/bindingindicator.cpp",
                "formeditor/bindingindicator.h",
                "formeditor/bindingindicatorgraphicsitem.cpp",
                "formeditor/bindingindicatorgraphicsitem.h",
                "formeditor/contentnoteditableindicator.cpp",
                "formeditor/contentnoteditableindicator.h",
                "formeditor/controlelement.cpp",
                "formeditor/controlelement.h",
                "formeditor/dragtool.cpp",
                "formeditor/dragtool.h",
                "formeditor/formeditor.qrc",
                "formeditor/formeditorgraphicsview.cpp",
                "formeditor/formeditorgraphicsview.h",
                "formeditor/formeditoritem.cpp",
                "formeditor/formeditoritem.h",
                "formeditor/formeditorscene.cpp",
                "formeditor/formeditorscene.h",
                "formeditor/formeditorview.cpp",
                "formeditor/formeditorview.h",
                "formeditor/formeditorwidget.cpp",
                "formeditor/formeditorwidget.h",
                "formeditor/itemutilfunctions.cpp",
                "formeditor/itemutilfunctions.h",
                "formeditor/layeritem.cpp",
                "formeditor/layeritem.h",
                "formeditor/lineeditaction.cpp",
                "formeditor/lineeditaction.h",
                "formeditor/movemanipulator.cpp",
                "formeditor/movemanipulator.h",
                "formeditor/movetool.cpp",
                "formeditor/movetool.h",
                "formeditor/numberseriesaction.cpp",
                "formeditor/numberseriesaction.h",
                "formeditor/onedimensionalcluster.cpp",
                "formeditor/onedimensionalcluster.h",
                "formeditor/resizecontroller.cpp",
                "formeditor/resizecontroller.h",
                "formeditor/resizehandleitem.cpp",
                "formeditor/resizehandleitem.h",
                "formeditor/resizeindicator.cpp",
                "formeditor/resizeindicator.h",
                "formeditor/resizemanipulator.cpp",
                "formeditor/resizemanipulator.h",
                "formeditor/resizetool.cpp",
                "formeditor/resizetool.h",
                "formeditor/rubberbandselectionmanipulator.cpp",
                "formeditor/rubberbandselectionmanipulator.h",
                "formeditor/scaleitem.cpp",
                "formeditor/scaleitem.h",
                "formeditor/scalemanipulator.cpp",
                "formeditor/scalemanipulator.h",
                "formeditor/selectionindicator.cpp",
                "formeditor/selectionindicator.h",
                "formeditor/selectionrectangle.cpp",
                "formeditor/selectionrectangle.h",
                "formeditor/selectiontool.cpp",
                "formeditor/selectiontool.h",
                "formeditor/singleselectionmanipulator.cpp",
                "formeditor/singleselectionmanipulator.h",
                "formeditor/snapper.cpp",
                "formeditor/snapper.h",
                "formeditor/snappinglinecreator.cpp",
                "formeditor/snappinglinecreator.h",
                "formeditor/toolbox.cpp",
                "formeditor/toolbox.h",
                "formeditor/zoomaction.cpp",
                "formeditor/zoomaction.h",
                "formeditor/formeditortoolbutton.cpp",
                "formeditor/formeditortoolbutton.h",
                "importmanager/importlabel.cpp",
                "importmanager/importlabel.h",
                "importmanager/importmanagercombobox.cpp",
                "importmanager/importmanagercombobox.h",
                "importmanager/importmanager.css",
                "importmanager/importmanager.qrc",
                "importmanager/importmanagerview.cpp",
                "importmanager/importmanagerview.h",
                "importmanager/importswidget.cpp",
                "importmanager/importswidget.h",
                "integration/componentaction.cpp",
                "integration/componentaction.h",
                "integration/componentview.cpp",
                "integration/componentview.h",
                "integration/designdocument.cpp",
                "integration/designdocument.h",
                "integration/designdocumentview.cpp",
                "integration/designdocumentview.h",
                "integration/stackedutilitypanelcontroller.cpp",
                "integration/stackedutilitypanelcontroller.h",
                "integration/utilitypanelcontroller.cpp",
                "integration/utilitypanelcontroller.h",
                "itemlibrary/itemlibrary.qrc",
                "itemlibrary/itemlibraryimageprovider.cpp",
                "itemlibrary/itemlibraryimageprovider.h",
                "itemlibrary/itemlibraryitem.cpp",
                "itemlibrary/itemlibraryitem.h",
                "itemlibrary/itemlibrarymodel.cpp",
                "itemlibrary/itemlibrarymodel.h",
                "itemlibrary/itemlibrarysection.cpp",
                "itemlibrary/itemlibrarysection.h",
                "itemlibrary/itemlibrarysectionmodel.cpp",
                "itemlibrary/itemlibrarysectionmodel.h",
                "itemlibrary/itemlibraryresourceview.cpp",
                "itemlibrary/itemlibraryresourceview.h",
                "itemlibrary/itemlibraryview.cpp",
                "itemlibrary/itemlibraryview.h",
                "itemlibrary/itemlibrarywidget.cpp",
                "itemlibrary/itemlibrarywidget.h",
                "itemlibrary/customfilesystemmodel.cpp",
                "itemlibrary/customfilesystemmodel.h",
                "navigator/iconcheckboxitemdelegate.cpp",
                "navigator/iconcheckboxitemdelegate.h",
                "navigator/nameitemdelegate.cpp",
                "navigator/nameitemdelegate.h",
                "navigator/navigator.qrc",
                "navigator/navigatortreemodel.cpp",
                "navigator/navigatortreemodel.h",
                "navigator/navigatortreeview.cpp",
                "navigator/navigatortreeview.h",
                "navigator/navigatorview.cpp",
                "navigator/navigatorview.h",
                "navigator/navigatorwidget.cpp",
                "navigator/navigatorwidget.h",
                "propertyeditor/designerpropertymap.cpp",
                "propertyeditor/designerpropertymap.h",
                "propertyeditor/fileresourcesmodel.cpp",
                "propertyeditor/fileresourcesmodel.h",
                "propertyeditor/gradientmodel.cpp",
                "propertyeditor/gradientmodel.h",
                "propertyeditor/propertyeditorcontextobject.cpp",
                "propertyeditor/propertyeditorcontextobject.h",
                "propertyeditor/propertyeditortransaction.cpp",
                "propertyeditor/propertyeditortransaction.h",
                "propertyeditor/propertyeditorvalue.cpp",
                "propertyeditor/propertyeditorvalue.h",
                "propertyeditor/propertyeditorview.cpp",
                "propertyeditor/propertyeditorview.h",
                "propertyeditor/propertyeditorwidget.cpp",
                "propertyeditor/propertyeditorwidget.h",
                "propertyeditor/propertyeditorqmlbackend.cpp",
                "propertyeditor/propertyeditorqmlbackend.h",
                "propertyeditor/quick2propertyeditorview.cpp",
                "propertyeditor/quick2propertyeditorview.h",
                "propertyeditor/qmlanchorbindingproxy.cpp",
                "propertyeditor/qmlanchorbindingproxy.h",
                "propertyeditor/qmlmodelnodeproxy.cpp",
                "propertyeditor/qmlmodelnodeproxy.h",
                "resources/resources.qrc",
                "stateseditor/stateseditorimageprovider.cpp",
                "stateseditor/stateseditorimageprovider.h",
                "stateseditor/stateseditormodel.cpp",
                "stateseditor/stateseditormodel.h",
                "stateseditor/stateseditorview.cpp",
                "stateseditor/stateseditorview.h",
                "stateseditor/stateseditorwidget.cpp",
                "stateseditor/stateseditorwidget.h",
            ]
        }

        Group {
            name: "extension"
            prefix: "qmldesignerextension/"
            files: [
                "colortool/colortool.cpp",
                "colortool/colortool.h",
                "connectioneditor/addnewbackenddialog.h",
                "connectioneditor/addnewbackenddialog.cpp",
                "connectioneditor/addnewbackenddialog.ui",
                "connectioneditor/backendmodel.h",
                "connectioneditor/backendmodel.cpp",
                "connectioneditor/delegates.h",
                "connectioneditor/delegates.cpp",
                "connectioneditor/bindingmodel.cpp",
                "connectioneditor/bindingmodel.h",
                "connectioneditor/connectioneditor.qrc",
                "connectioneditor/connectionmodel.cpp",
                "connectioneditor/connectionmodel.h",
                "connectioneditor/connectionview.cpp",
                "connectioneditor/connectionview.h",
                "connectioneditor/connectionviewwidget.cpp",
                "connectioneditor/connectionviewwidget.h",
                "connectioneditor/connectionviewwidget.ui",
                "connectioneditor/dynamicpropertiesmodel.cpp",
                "connectioneditor/dynamicpropertiesmodel.h",
                "connectioneditor/stylesheet.css",
                "pathtool/controlpoint.cpp",
                "pathtool/controlpoint.h",
                "pathtool/cubicsegment.cpp",
                "pathtool/cubicsegment.h",
                "pathtool/pathitem.cpp",
                "pathtool/pathitem.h",
                "pathtool/pathselectionmanipulator.cpp",
                "pathtool/pathselectionmanipulator.h",
                "pathtool/pathtool.cpp",
                "pathtool/pathtool.h",
                "pathtool/pathtoolview.cpp",
                "pathtool/pathtoolview.h",
                "qmldesignerextensionconstants.h",
                "qmldesignerextension_global.h",
                "sourcetool/sourcetool.cpp",
                "sourcetool/sourcetool.h",
                "texttool/textedititem.cpp",
                "texttool/textedititem.h",
                "texttool/textedititemwidget.cpp",
                "texttool/textedititemwidget.h",
                "texttool/texttool.cpp",
                "texttool/texttool.h",
            ]
        }

        files: [
            "designersettings.cpp",
            "designersettings.h",
            "designmodecontext.cpp",
            "designmodecontext.h",
            "designmodewidget.cpp",
            "designmodewidget.h",
            "switchsplittabwidget.cpp",
            "switchsplittabwidget.h",
            "documentmanager.cpp",
            "documentmanager.h",
            "documentwarningwidget.cpp",
            "documentwarningwidget.h",
            "openuiqmlfiledialog.cpp",
            "openuiqmlfiledialog.h",
            "openuiqmlfiledialog.ui",
            "qmldesignerconstants.h",
            "qmldesignericons.h",
            "qmldesignerplugin.cpp",
            "qmldesignerplugin.h",
            "settingspage.cpp",
            "settingspage.h",
            "settingspage.ui",
            "shortcutmanager.cpp",
            "shortcutmanager.h",
        ]
    }
}
