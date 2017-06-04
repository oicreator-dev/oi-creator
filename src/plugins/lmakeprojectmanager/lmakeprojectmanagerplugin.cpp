#include "lmakeprojectmanagerplugin.h"

using namespace LmakeProjectManager;

bool LmakeProjectManagerPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)
    return true;
}

void LmakeProjectManagerPlugin::extensionsInitialized()
{

}

LmakeProjectManagerPlugin::LmakeProjectManagerPlugin()
{

}
