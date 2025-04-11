#ifndef MUSIC_PLAYER_IMPROVED_CONTROLLER_H
#define MUSIC_PLAYER_IMPROVED_CONTROLLER_H
#include "../model/Model.h"
#include "../view/View.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QStack>

class Controller : public QObject {
Q_OBJECT

private:
    Model *model_;
    View *view_;
    QMediaPlayer *media_player_;
    QAudioOutput *audio_output_;
    QStack<int> song_stack_;

public:
    explicit Controller(Model *model, View *view, QObject *parent = nullptr);

public slots:
    void playSelectedSong();
    void playNextSong();
    void playPreviousSong();
    void addSong();
    void removeSong();
    void playRandomAd();
    void skipAd();
    void updateVolume(int volume);
    void handleMediaStatus(QMediaPlayer::MediaStatus status);
    void sortByNumber();
    void sortByName();
};

#endif //MUSIC_PLAYER_IMPROVED_CONTROLLER_H
