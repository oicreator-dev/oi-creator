#include "lmakeprojectmanager.h"
#include <QDebug>

using namespace LmakeProjectManager;

LmakeManager::LmakeManager(QObject *parent) : QObject(parent)
{

}

void LmakeManager::runLMake()
{
    qDebug()<<"runLMake run";
}
