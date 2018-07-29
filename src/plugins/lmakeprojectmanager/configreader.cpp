#include "configreader.h"

#include "core.h"
#include "data.h"
#include "error.h"
#include "stringtools.h"
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTextStream>

QStringList readFromConfigFile(QFile &file)
{
    QTextStream ts(file.readAll(), QIODevice::ReadOnly);
    QStringList code;
    while (!ts.atEnd()) {
        code.push_back(ts.readLine());
    }
    return code;
}

QStringList readFromSourceFile(QFile &file)
{
    QTextStream ts(file.readAll(), QIODevice::ReadOnly);
    QStringList code;
    bool isLMCode = false;
    while (!ts.atEnd()) {
        QString line = ts.readLine();
        if (isLMCode) {
            line = removeSpace(line);
            if (line.length() >= 2 && line[0] == '*' && line[1] == '/') {
                isLMCode = false;
                code.push_back(QString());
                continue;
            }
            int i;
            for (i = 0; i < line.length() && (line[i] == ' ' || line[i] == '\t' || line[i] == '*');
                 i++)
                ;
            line = line.mid(i);
            code.push_back(line);
        } else {
            line = removeSpace(line);
            if (line.left(8) == "/*+lmake") {
                isLMCode = true;
            }
            code.push_back(QString());
        }
    }
    if (isLMCode)
        throw Error("Unexpected file end.").setLine(code.size());
    return code;
}

bool interpretCode(const QStringList &code)
{
    QStringList pCode = preprocessCode(code);
    int line = 0;
    for (const QString &i : pCode) {
        ++line;
        if (i.isEmpty())
            continue;
        QStringList x = i.split(' ');
        if (x.size() < 2)
            throw Error(QObject::tr("Unexpected end.")).setLine(line);
        QString variableName = x[0];
        if (!isRightVariableName(variableName))
            throw Error(QObject::tr("Unlawful variable name")).setLine(line);
        variableName = variableName.toLower();
        QString op = x[1];
        if (op == "=") {
            QVariantList valueList;
            for (int i = 2; i < x.size(); ++i) {
                valueList.push_back(x[i]);
            }
            Data::setVariable(variableName, valueList);
        } else if (op == "+=") {
            QVariantList valueList;
            for (int i = 2; i < x.size(); ++i) {
                valueList.push_back(x[i]);
            }
            Data::appendVariable(variableName, valueList);
        } else if (op == "-=") {
            //TODO:
        } else if (op == "++") {
            if (x.size() > 2)
                throw Error(QObject::tr("too many value after operator ++")).setLine(line);
            if (!Data::isVariable(variableName))
                Data::addVariable(variableName);
        } else if (op == "--") {
            if (x.size() > 2)
                throw Error(QObject::tr("too many value after operator --")).setLine(line);
            if (Data::isVariable(variableName))
                Data::deleteVariable(variableName);
        } else
            throw Error(QObject::tr("Unknown operator %1").arg(op)).setLine(line);
    }
    return true;
}

QStringList preprocessCode(const QStringList &code)
{
    QStringList preprocessedCode;
    for (const QString &i : code) {
        QString line = i;
        int pos = line.indexOf('#');
        if (pos != -1) {
            line = line.mid(0, pos);
        }
        line = removeSpace(line);
        //if (!line.isEmpty() && line[line.size()-1]=='\\')
        preprocessedCode.push_back(line);
    }
    return preprocessedCode;
}

bool isRightVariableName(const QString &code)
{
    Q_UNUSED(code)
    return true;
}
