#include "stringtools.h"

#include <QRegExp>
#include <QString>

namespace LMake {

QString removeExtension(const QString &source)
{
    QString pattern(R"(^(.*)\.[^/\\.]*$)");
    QRegExp rx(pattern);
    if (rx.exactMatch(source)) {
        return rx.cap(1);
    }
    return QString();
}

QString removeSpace(const QString &code)
{
    QString formattedCode;
    bool isSpace = false;
    for (const QChar &it : code) {
        if (isSpace) {
            if (it == ' ' || it == '\t' || it == QChar('\0'))
                continue;
            if (!formattedCode.isEmpty())
                formattedCode.append(' ');
            formattedCode.append(it);
            isSpace = false;
        } else {
            if (it == ' ' || it == '\t' || it == QChar('\0'))
                isSpace = true;
            else
                formattedCode.append(it);
        }
    }
    return formattedCode;
}
} // namespace LMake
