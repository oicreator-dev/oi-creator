#include "lmakeprojectmanager.h"
#include "lmakeapi.h"
#include "stringtools.h"

#include <coreplugin/editormanager/editormanager.h>

#include <QByteArray>
#include <QDebug>
#include <QProcess>
#include <QString>
#include <QStringList>

namespace LmakeProjectManager {

LmakeManager::LmakeManager(QObject *parent)
    : QObject(parent)
{
    m_lmakeapi = new LMake::LMakeAPI;
}

bool LmakeManager::runLMake()
{
    QString currentFileName = Core::EditorManager::currentDocument()->filePath().toString();
    QStringList lmakeMessage = m_lmakeapi->runLMake(currentFileName);
    qDebug() << lmakeMessage;
    if (!lmakeMessage.empty()) {
    }
    return lmakeMessage.empty();
}

bool LmakeManager::build()
{
    if (!runLMake())
        return false;
    QString currentFileName = Core::EditorManager::currentDocument()->filePath().toString();
    QProcess makeProcess;
    makeProcess.start(QString("make -f %1.Makefile").arg(LMake::removeExtension(currentFileName)));
    makeProcess.waitForFinished(60 * 1000);
    QByteArray stdOut = makeProcess.readAllStandardOutput();
    QByteArray stdError = makeProcess.readAllStandardError();
    qDebug() << stdOut;
    qDebug() << stdError;
    return (stdError.size() == 0);
}
} // namespace LmakeProjectManager
