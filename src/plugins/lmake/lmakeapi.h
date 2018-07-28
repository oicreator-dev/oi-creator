#pragma once

#include <QObject>

namespace LMake
{
class LMakeAPI : public QObject
{
    Q_OBJECT
public:
    explicit LMakeAPI(QObject *parent = 0);
};
}
