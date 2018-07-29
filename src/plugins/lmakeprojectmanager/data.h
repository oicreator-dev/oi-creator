#pragma once

#include <QMap>
#include <QString>
#include <QVariantList>

/*! \class Data
  \since 0.0.1
  \brief Data类存储解析出来的变量和函数等内容,以及一些对他们的操作

 */
class Data
{
public:
    Data();
    static bool setVariable(const QString &name, const QVariantList &value);
    static bool setVariable(const QString &name, const QVariant &value);
    static bool appendVariable(const QString &name, const QVariant &value);
    static bool appendVariable(const QString &name, const QVariantList &value);
    static bool deleteVariable(const QString &name);
    static bool isVariable(const QString &name);
    static bool isValueInVariable(const QString &name, const QVariant &value);
    static bool addVariable(const QString &name);
    static const QVariantList &getVariable(const QString &name);

#ifdef QT_DEBUG
    static QMap<QString, QVariantList> debugOnly_getVariableMap();
#endif
private:
    static QMap<QString, QVariantList> m_variable;
};
