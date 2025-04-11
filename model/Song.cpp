#include "Song.h"
#include <qregularexpression.h>

Song::Song(int number, const QString &name, const QString &file_path) : number_(number), name_(name),
file_path_(file_path) {}

int Song::get_number() const {
    QRegularExpression numberRegex(R"(\((\d+)\)\s.+)");
    QRegularExpressionMatch match = numberRegex.match(name_);
    if (match.hasMatch()) {
        return match.captured(1).toInt();
    }
    return number_;
}

QString Song::get_name() const {
    QRegularExpression nameRegex(R"(\(\d+\)\s(.+))");
    QRegularExpressionMatch match = nameRegex.match(name_);
    if (match.hasMatch()) {
        return match.captured(1).trimmed();
    }
    return name_.trimmed();
}

QString Song::get_file_path() const {
    return file_path_;
}
