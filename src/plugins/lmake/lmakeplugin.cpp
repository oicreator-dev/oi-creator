#include "lmakeplugin.h"

#include "lmakeapi.h"

#include <coreplugin/icore.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/command.h>

#include <projectexplorer/projectexplorerconstants.h>

using namespace LMake;

using namespace Core;
using namespace ProjectExplorer;
namespace LMake {
static LMakeAPI* lmakeapi=nullptr;
bool LMakePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    const Context projectContext("Qt4ProjectManager.Qt4Project");

    lmakeapi = new LMakeAPI;

    return true;
}

void LMakePlugin::extensionsInitialized()
{

}

LMakePlugin::LMakePlugin()
{

}

LMakeAPI *LMakePlugin::getLMakeAPI()
{
    return lmakeapi;
}
}
