#include "oiwikiplugin.h"

#include "wikimode.h"

#include <coreplugin/icore.h>

#include <QAction>

namespace OIWiki {

bool OIWikiPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    const Core::Context projectContext("Qt4ProjectManager.Qt4Project");

    return true;
}

void OIWikiPlugin::extensionsInitialized() {
    WikiMode::createModeIfRequired();
}

bool OIWikiPlugin::delayedInitialize()
{
    return true;
}

OIWikiPlugin::OIWikiPlugin() {}

OIWikiPlugin::~OIWikiPlugin()
{
   WikiMode::destroyModeIfRequired();
}
} // namespace OIWiki
