#pragma once

#include "lmakeapi.h"

#include <QObject>

namespace LmakeProjectManager {
class LmakeManager : public QObject
{
    Q_OBJECT
public:
    explicit LmakeManager(QObject *parent = 0);

    bool runLMake();
    bool build();
signals:

public slots:
private:
    LMake::LMakeAPI *m_lmakeapi = nullptr;
};
} // namespace LmakeProjectManager
