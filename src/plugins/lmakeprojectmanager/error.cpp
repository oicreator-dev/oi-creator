#include "error.h"

#include <QObject>
#include <QString>
#ifdef QT_DEBUG
#include <QDebug>
#endif

Error::Error(const QString &errorMessage, int line, const QString &fileName)
    : m_errorMessage(errorMessage)
    , m_line(line)
    , m_fileName(fileName)
{}

QString Error::toString()
{
    QString message;
    if (!m_fileName.isEmpty())
        message.append(m_fileName + ": ");
    if (m_line != -1)
        message.append(QString::number(m_line) + ": ");
    return message.append(m_errorMessage);
}

Error &Error::setLine(int line)
{
    m_line = line;
    return *this;
}

Error &Error::setFileName(const QString &fileName)
{
    m_fileName = fileName;
    return *this;
}
