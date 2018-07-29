#include "lmakeprojectmanagerplugin.h"
#include "lmakeprojectmanager.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icore.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <QAction>

using namespace LmakeProjectManager;

using namespace Core;
using namespace ProjectExplorer;

bool LmakeProjectManagerPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    const Context projectContext("Qt4ProjectManager.Qt4Project");

    m_lmakeProjectManager = new LmakeManager;

    ActionContainer *mbuild = //build菜单
        ActionManager::actionContainer(ProjectExplorer::Constants::M_BUILDPROJECT);

    Command *command = nullptr; //似乎是用来保存命令的

    m_runLMakeAction = new QAction(tr("Run lmake"), this);
    const Context globalcontext(Core::Constants::C_GLOBAL);
    command = ActionManager::registerAction(m_runLMakeAction, "Qt4Builder.RunLMake", globalcontext);
    mbuild->addAction(command, ProjectExplorer::Constants::G_BUILD_BUILD);
    connect(m_runLMakeAction, &QAction::triggered, m_lmakeProjectManager, &LmakeManager::runLMake);

    return true;
}

void LmakeProjectManagerPlugin::extensionsInitialized() {}

LmakeProjectManagerPlugin::LmakeProjectManagerPlugin() {}
