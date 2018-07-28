#pragma once

#include <QObject>
#include "lmakeapi.h"

namespace LmakeProjectManager
{
class LmakeManager : public QObject
{
    Q_OBJECT
public:
    explicit LmakeManager(QObject *parent = 0);

    void runLMake();
signals:

public slots:
private:
    LMake::LMakeAPI* m_lmakeapi=nullptr;
};
}
