#ifndef MUSIC_PLAYER_IMPROVED_SONG_H
#define MUSIC_PLAYER_IMPROVED_SONG_H
#include <QString>

class Song {
private:
    int number_;
    QString name_;
    QString author_;
    QString album_;
    QString cover_;
    QString genre_;
    QString file_path_;

public:
    Song(int number, const QString &name, const QString &filePath);

    int getNumber() const;
    QString getName() const;
    QString getFilePath() const;
};

#endif //MUSIC_PLAYER_IMPROVED_SONG_H
