#pragma once

#include "error.h"
template<typename>
class QList;
class QVariant;
using QVariantList = QList<QVariant>;
class QString;

bool checkVariableSize(const QString &name, int min, int max, int mode = Error::THROW_ERROR);
QString generateMakeFile();
