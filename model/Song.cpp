#include "Song.h"
#include <regex.h>
#include <qregularexpression.h>

Song::Song(int number, const QString &name, const QString &filePath) : number(number), name(name),
filePath(filePath) {}

int Song::getNumber() const {
    QRegularExpression numberRegex(R"(\((\d+)\)\s.+)");
    QRegularExpressionMatch match = numberRegex.match(name);
    if (match.hasMatch()) {
        return match.captured(1).toInt();
    }
    return number;
}

QString Song::getName() const {
    QRegularExpression nameRegex(R"(\(\d+\)\s(.+))");
    QRegularExpressionMatch match = nameRegex.match(name);
    if (match.hasMatch()) {
        return match.captured(1).trimmed();
    }
    return name.trimmed();
}

QString Song::getFilePath() const {
    return filePath;
}
