#pragma once

#include <coreplugin/icontext.h>
#include <extensionsystem/iplugin.h>


namespace OIWiki {
class OIWikiPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "OIWiki.json")
public:
    bool initialize(const QStringList &arguments, QString *errorMessage) override;
    void extensionsInitialized() override;
    bool delayedInitialize() override;

public:
    OIWikiPlugin();
};
} // namespace OIWiki
