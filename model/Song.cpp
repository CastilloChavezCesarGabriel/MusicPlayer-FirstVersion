#include "Song.h"
#include <qregularexpression.h>

Song::Song(int number, const QString &name, const QString &filePath) : number_(number), name_(name),
file_path_(filePath) {}

int Song::getNumber() const {
    QRegularExpression numberRegex(R"(\((\d+)\)\s.+)");
    QRegularExpressionMatch match = numberRegex.match(name_);
    if (match.hasMatch()) {
        return match.captured(1).toInt();
    }
    return number_;
}

QString Song::getName() const {
    QRegularExpression nameRegex(R"(\(\d+\)\s(.+))");
    QRegularExpressionMatch match = nameRegex.match(name_);
    if (match.hasMatch()) {
        return match.captured(1).trimmed();
    }
    return name_.trimmed();
}

QString Song::getFilePath() const {
    return file_path_;
}
