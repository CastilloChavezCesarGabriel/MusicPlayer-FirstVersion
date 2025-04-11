#ifndef MUSIC_PLAYER_IMPROVED_MODEL_H
#define MUSIC_PLAYER_IMPROVED_MODEL_H
#include <QObject>
#include <QStringList>
#include <QDir>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QRandomGenerator>
#include <QVector>
#include <algorithm>
#include <random>
#include "Sorting.h"
#include "Song.h"

class Model final : public QObject {
Q_OBJECT

private:
    QStringList songList;
    QVector<Song> songs;
    QStringList adFiles;
    QString resourcesPath;
    QString announcementsPath;
    bool playingAd = false;
    Sorting sort;
    QString lastSortMethod;

public:
    explicit Model(QObject *parent = nullptr);

    QStringList getSongs() const { return songList; }
    bool isPlayingAd() const { return playingAd; }
    QString getSongAt(int index) const;

    void add(const QString &filePath);
    void remove(const QString &filePath);
    void loadAdFiles();
    QString getRandomAd() const;
    void setPlayingAd(bool state) { playingAd = state; }
    void dropFiles(const QList<QUrl> &urls);
    void sortByNumber();
    void sortByName();
    void shuffle();
    void updateList();

signals:
    void songsUpdated(const QStringList &songs);
    void adsUpdated(const QStringList &ads);
    void disableButtons();

private:
    void loadSongs();
    void saveToResources(const QString &filePath);
    QStringList getExtension(const QString &directionPath) const;
    QString getMusicPath() const;
    QString getAnnouncementsPath() const;
};

#endif //MUSIC_PLAYER_IMPROVED_MODEL_H
