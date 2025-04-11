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
    Song(int number, const QString &name, const QString &file_path);

    int get_number() const;
    QString get_name() const;
    QString get_file_path() const;
};

#endif //MUSIC_PLAYER_IMPROVED_SONG_H
