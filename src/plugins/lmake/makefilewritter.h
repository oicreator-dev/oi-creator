#ifndef MAKEFILEWRITTER_H
#define MAKEFILEWRITTER_H

#include "error.h"
template <typename> class QList;
class QVariant;
using QVariantList=QList<QVariant>;
class QString;

bool checkVariableSize(const QString& name,int min,int max,int mode=Error::THROW_ERROR);
QString generateMakeFile();
#endif // MAKEWRITTER_H
