#pragma once

#include <QObject>

namespace LMake {
class LMakeAPI : public QObject
{
    Q_OBJECT
public:
    explicit LMakeAPI(QObject *parent = nullptr);
    QStringList runLMake(QString fileName);
};
} // namespace LMake
