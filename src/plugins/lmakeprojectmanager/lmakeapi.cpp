#include "lmakeapi.h"
#include "configreader.h"
#include "core.h"
#include "data.h"
#include "error.h"
#include "makefilewritter.h"
#include "stringtools.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QUrl>

namespace LMake {
LMakeAPI::LMakeAPI(QObject *parent)
    : QObject(parent)
{}

QStringList LMakeAPI::runLMake(QString fileName)
{
    try {
        QString sourceFileString = fileName;
        QUrl sourceFileUrl;

        if (sourceFileString.isEmpty())
            throw Error(QObject::tr("no source file"));
        sourceFileUrl.setUrl(sourceFileString);
        try {
            QFile configFile;
            configFile.setFileName(":/config/default.lm");
            configFile.open(QIODevice::ReadOnly);
            QStringList config = readFromConfigFile(configFile);
            interpretCode(config);
        } catch (Error &e) {
            throw e.setFileName("default.lm");
        }
        Data::appendVariable("SOURCES", sourceFileString);
        QString noExtensionSrc = removeExtension(sourceFileString);
        if (noExtensionSrc.isEmpty())
            noExtensionSrc = sourceFileString + ".cpp";
#ifdef Q_OS_WIN
        Data::setVariable("OUTPUT", noExtensionSrc + ".exe");
#else
        Data::setVariable("OUTPUT", noExtensionSrc);
#endif
        try {
            QFileInfo sourceFileInfo(sourceFileString);
            if (!sourceFileInfo.exists())
                throw Error(QObject::tr("No such file")).setFileName(sourceFileString);
            QFile sourceFile;
            sourceFile.setFileName(sourceFileString);
            sourceFile.open(QIODevice::ReadOnly);
            QStringList source = readFromSourceFile(sourceFile);
            interpretCode(source);
        } catch (Error &e) {
            throw e.setFileName(sourceFileString);
        }
        try {
            QString make = generateMakeFile();
            QFile makeFile;
            makeFile.setFileName(removeExtension(sourceFileString) + ".Makefile");
            makeFile.open(QIODevice::WriteOnly);
            makeFile.write(make.toLocal8Bit());
        } catch (Error &e) {
            throw e;
        }
    } catch (Error &e) {
        return QStringList() << e.toString();
    } catch (...) {
        return QStringList() << QObject::tr("Unknown Error!");
    }
    return QStringList();
}
} // namespace LMake
