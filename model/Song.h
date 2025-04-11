#ifndef MUSIC_PLAYER_IMPROVED_SONG_H
#define MUSIC_PLAYER_IMPROVED_SONG_H
#include <QString>

class Song {
private:
    int number;
    QString name;
    QString author;
    QString album;
    QString cover;
    QString genre;
    QString filePath;

public:
    Song(int number, const QString &name, const QString &filePath);

    int getNumber() const;
    QString getName() const;
    QString getFilePath() const;
};

#endif //MUSIC_PLAYER_IMPROVED_SONG_H
