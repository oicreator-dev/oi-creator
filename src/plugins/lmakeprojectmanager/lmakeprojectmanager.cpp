#include "lmakeprojectmanager.h"
#include "lmakeapi.h"
#include <coreplugin/editormanager/editormanager.h>

#include <QByteArray>
#include <QDebug>
#include <QProcess>
#include <QString>
#include <QStringList>

using namespace LmakeProjectManager;

LmakeManager::LmakeManager(QObject *parent)
    : QObject(parent)
{
    m_lmakeapi = new LMake::LMakeAPI;
}

void LmakeManager::runLMake()
{
    QString currentFileName = Core::EditorManager::currentDocument()->filePath().toString();
    //LMake::LMakePlugin::getLMakeAPI();
    QStringList lmakeMessage = m_lmakeapi->runLMake(currentFileName);
    qDebug() << lmakeMessage;
    if (lmakeMessage.empty()) {
        QProcess makeProcess;
        makeProcess.start(QString("make -f %1").arg(currentFileName + ".Makefile"));
        makeProcess.waitForFinished(60 * 1000);
        QByteArray t = makeProcess.readAllStandardOutput();
        qDebug() << t;
        qDebug() << makeProcess.readAllStandardError();
    } else {
    }
}
