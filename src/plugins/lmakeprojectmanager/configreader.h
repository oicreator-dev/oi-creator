#pragma once

class QString;
class QFile;
class QStringList;

QStringList readFromConfigFile(QFile &file);
QStringList readFromSourceFile(QFile &file);

QStringList preprocessCode(const QStringList &code);
bool interpretCode(const QStringList &code);

bool isRightVariableName(const QString &code);
