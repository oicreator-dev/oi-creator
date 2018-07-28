#pragma once

#include <extensionsystem/iplugin.h>
#include <coreplugin/icontext.h>
namespace LMake
{
class LMakeAPI;
class LMakePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Lmake.json")
public:
    bool initialize(const QStringList &arguments, QString *errorMessage) override;
    void extensionsInitialized() override;
public:
    LMakePlugin();
    static LMakeAPI* getLMakeAPI();
};
}

