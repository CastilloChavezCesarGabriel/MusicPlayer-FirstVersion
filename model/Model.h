#ifndef MUSIC_PLAYER_IMPROVED_MODEL_H
#define MUSIC_PLAYER_IMPROVED_MODEL_H
#include <QObject>
#include <QStringList>
#include <QDir>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <algorithm>
#include <random>
#include "Sorting.h"
#include "Song.h"

class Model final : public QObject {
Q_OBJECT
private:
    QStringList song_list_;
    QVector<Song> songs_;
    QStringList ad_files_;
    QString resources_path_;
    QString announcements_path_;
    bool playing_ad_ = false;
    Sorting sort_;
    QString last_sort_method_;

public:
    explicit Model(QObject *parent = nullptr);

    QStringList get_songs() const { return song_list_; }
    bool is_playing_ad() const { return playing_ad_; }
    QString get_song_at(int index) const;

    void add(const QString &file_path);
    void remove(const QString &file_path);
    void load_ad_files();
    QString get_random_ad() const;
    void set_playing_ad(bool state) { playing_ad_ = state; }
    void drop_files(const QList<QUrl> &urls);
    void sort_by_number();
    void sort_by_name();
    void shuffle();
    void update_list();

signals:
    void songs_updated(const QStringList &songs);
    void ads_updated(const QStringList &ads);
    void disable_buttons();

private:
    void load_songs();
    static void save_to_resources(const QString &file_path);
    static QStringList get_extension(const QString &direction_path);
    static QString get_music_path();
    static QString get_announcements_path();
};

#endif //MUSIC_PLAYER_IMPROVED_MODEL_H
