#include "makefilewritter.h"

#include "data.h"
#include "error.h"
#include "stringtools.h"

#include <QObject>
#include <QRegExp>
#include <QString>
#include <QTextStream>
#include <QVariant>
#include <QVariantList>

QString generateMakeFile()
{
    QString str;
    QTextStream ts(&str, QIODevice::WriteOnly);
    checkVariableSize("TEMPLATE", 1, 1);
    QVariantList Template = Data::getVariable("TEMPLATE");
    QString option;
    if (checkVariableSize("DEFINE", 1, 100000, Error::DO_NOTHING)) {
        QVariantList var = Data::getVariable("DEFINE");
        for (const QVariant &i : var) {
            option.append(QString(" -D%1").arg(i.toString()));
        }
    }
    if (checkVariableSize("STD", 1, 10000, Error::DO_NOTHING)) {
        QString var = Data::getVariable("STD").at(0).toString();
        if (var == "c++11" || var == "c++0x")
            option.append(" -std=c++11");
        else if (var == "c++14")
            option.append(" -std=c++14");
        else if (var == "c++17" || var == "c++1z")
            option.append(" -std=c++17");
        else if (var == "c++98")
            option.append(" -std=c++98");
        else {
            //TODO: throw warning
        }
    }
    if (checkVariableSize("DEBUG_MODE", 1, 10000, Error::DO_NOTHING)) {
        QVariant var = Data::getVariable("DEBUG_MODE").at(0);
        if (var.toString() == "true") {
            option.append(" -g");
        } else if (var.toString() == "false")
            ;
        else {
            //TODO: throw warning
        }
    }

    checkVariableSize("GCC_PATH", 1, 1);
    QString gccPath = Data::getVariable("GCC_PATH").at(0).toString();

    QString optimize = QString();
    if (checkVariableSize("OPTIMIZE", 1, 10000, Error::DO_NOTHING)) {
        optimize = Data::getVariable("OPTIMIZE").at(0).toString();
    }
    if (!optimize.isEmpty())
        option.push_back(QString(" -O%1").arg(optimize));

    if (checkVariableSize("CXX_FLAGS", 1, 100000, Error::DO_NOTHING)) {
        const QVariantList &cxx_flags = Data::getVariable("CXX_FLAGS");
        for (const QVariant &i : cxx_flags) {
            option.push_back(" " + i.toString());
        }
    }

    if (Template[0].toString() == "default") {
        checkVariableSize("OUTPUT", 1, 1);
        const QString output = Data::getVariable("OUTPUT").at(0).toString();
        checkVariableSize("SOURCES", 1, 10000);
        const QVariantList &sources = Data::getVariable("SOURCES");

        ts << QString("%1 :").arg(output);
        for (const QVariant &i : sources) {
            QString objFileName = removeExtension(i.toString()) + ".o";
            ts << QString(" %1").arg(objFileName);
        }

#ifdef Q_OS_WIN
        QString stack;
        if (checkVariableSize("STACK", 1, 10000, Error::DO_NOTHING)) {
            bool ok;
            int var = Data::getVariable("DEBUG_MODE").at(0).toInt(&ok);
            if (!ok) {
                //TODO: throw warning

            } else {
                stack = QString(" -Wl,--stack=%1").arg(var);
            }
        }
#endif

        ts << QString("\n\t%2 -o %1").arg(output).arg(gccPath);
        for (const QVariant &i : sources) {
            QString objFileName = removeExtension(i.toString()) + ".o";
            ts << QString(" %1").arg(objFileName);
        }
#ifdef Q_OS_WIN
        ts << stack;
#endif
        ts << option;
        ts << '\n';

        for (const QVariant &i : sources) {
            QString objFileName = removeExtension(i.toString()) + ".o";
            ts << QString("%1 : %2\n\tg++ -o %3 -c %2")
                      .arg(objFileName, i.toString())
                      .arg(objFileName);
            ts << option;
            ts << "\n";
        }
        QStringList rmList;
        rmList << output + ".Makefile";
        for (const QVariant &i : sources) {
            rmList << removeExtension(i.toString()) + ".o";
        }
#ifdef Q_OS_LINUX
        ts << QString("clean:\n\trm");
        for (const QString &i : rmList) {
            ts << QString(" %1").arg(i);
        }
        ts << "\n";

        rmList << output;
        ts << QString("full_clean:\n\trm");
        for (const QString &i : rmList) {
            ts << QString(" %1").arg(i);
        }
#elif defined(Q_OS_WIN)
        ts << QString("clean:");
        for (const QString &i : rmList) {
            ts << QString("\n\tdel %1 /q").arg(i);
        }
        ts << "\n";

        rmList << output;
        ts << QString("clean:");
        for (const QString &i : rmList) {
            ts << QString("\n\tdel %1 /q").arg(i);
        }
#endif

    } else
        Error(
            QObject::tr("Unknown value %2 in variable %1").arg("TEMPLATE", Template[0].toString()));
    return str;
}

bool checkVariableSize(const QString &name, int min, int max, int mode)
{
    if (mode == Error::DO_NOTHING) {
        if (!Data::isVariable(name))
            return false;
        const QVariantList &var = Data::getVariable(name);
        if (var.size() > max)
            return false;
        if (var.size() < min)
            return false;
        return true;
    }
    if (!Data::isVariable(name))
        throw Error(QObject::tr("varibale %1 not foud").arg(name));
    const QVariantList &var = Data::getVariable(name);
    if (var.size() > max)
        throw Error(QObject::tr("too much value in varibale %1").arg(name));
    if (var.size() < min)
        throw Error(QObject::tr("too few value in varibale %1").arg(name));
    return true;
}
