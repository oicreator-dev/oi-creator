#ifndef LMAKEPROJECTMANAGERPLUGIN_H
#define LMAKEPROJECTMANAGERPLUGIN_H

#include <extensionsystem/iplugin.h>
#include <coreplugin/icontext.h>

class LmakeProjectManagerPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "LmakeProjectManager.json")
public:
    bool initialize(const QStringList &arguments, QString *errorMessage) override;
public:
    LmakeProjectManagerPlugin();
};

#endif // LMAKEPROJECTMANAGERPLUGIN_H
