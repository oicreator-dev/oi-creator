#ifndef LMAKEPROJECTMANAGER_H
#define LMAKEPROJECTMANAGER_H

#include <QObject>

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
};
}

#endif // LMAKEPROJECTMANAGER_H
