#pragma once

#include <QString>

class Error
{
public:
    enum ExceptionMode { DO_NOTHING = 0, THROW_ERROR = 1, THROW_WORNING = 2 };

public:
    Error(const QString &errorMessage = QString(),
          int line = -1,
          const QString &fileName = QString());
    QString toString();
    Error &setLine(int line);
    Error &setFileName(const QString &fileName);

private:
    QString m_errorMessage;
    int m_line;
    QString m_fileName;
};
