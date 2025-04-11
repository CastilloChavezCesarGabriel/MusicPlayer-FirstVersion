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
public:
    explicit Controller(Model *model, View *view, QObject *parent = nullptr);

public slots:
    void play_selected_song();
    void play_next_song();
    void play_previous_song();
    void add_song() const;
    void remove_song() const;
    void play_random_ad();
    void skip_ad();
    void update_volume(int volume) const;
    void handle_media_status(QMediaPlayer::MediaStatus status);
    void sort_by_number() const;
    void sort_by_name() const;

private:
    Model *model_;
    View *view_;
    QMediaPlayer *media_player_;
    QAudioOutput *audio_output_;
    QStack<int> song_stack_;
};

#endif //MUSIC_PLAYER_IMPROVED_CONTROLLER_H
