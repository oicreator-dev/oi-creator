#include "lmakeprojectmanager.h"
#include <coreplugin/editormanager/editormanager.h>
#include "lmakeapi.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QByteArray>

using namespace LmakeProjectManager;

LmakeManager::LmakeManager(QObject *parent) : QObject(parent)
{
    m_lmakeapi=new LMake::LMakeAPI;
}

void LmakeManager::runLMake()
{
    QString currentFileName=Core::EditorManager::currentDocument()->filePath().toString();
    //LMake::LMakePlugin::getLMakeAPI();
    QStringList lmakeMessage= m_lmakeapi->runLMake(currentFileName);
    qDebug()<<lmakeMessage;
    if (lmakeMessage.empty())  {
        QProcess makeProcess;
        makeProcess.start(QString("make -f %1").arg(currentFileName+".Makefile"));
        makeProcess.waitForFinished(60*1000);
        QByteArray t=makeProcess.readAllStandardOutput();
        qDebug()<<t;
        qDebug()<<makeProcess.readAllStandardError();
    } else {

    }
}
