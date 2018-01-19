#pragma once

#include <extensionsystem/iplugin.h>
#include <coreplugin/icontext.h>
namespace LmakeProjectManager
{
class LmakeManager;
class LmakeProjectManagerPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "LmakeProjectManager.json")
public:
    bool initialize(const QStringList &arguments, QString *errorMessage) override;
    void extensionsInitialized() override;
public:
    LmakeProjectManagerPlugin();
private:
    QAction *m_runLMakeAction=nullptr;
    LmakeManager *m_lmakeProjectManager=nullptr;
};
}

