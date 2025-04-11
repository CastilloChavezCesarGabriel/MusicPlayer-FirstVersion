#include "Controller.h"
#include <QTimer>
#include <QRandomGenerator>

Controller::Controller(Model *model, View *view, QObject *parent)
        : QObject(parent), model_(model), view_(view) {

    media_player_ = new QMediaPlayer(this);
    audio_output_ = new QAudioOutput(this);
    media_player_->setAudioOutput(audio_output_);

    connect(view->btn_play, &QPushButton::clicked, this, &Controller::playSelectedSong);
    connect(view->btn_pause, &QPushButton::clicked, media_player_, &QMediaPlayer::pause);
    connect(view->btn_stop, &QPushButton::clicked, media_player_, &QMediaPlayer::stop);
    connect(view->btn_next, &QPushButton::clicked, this, &Controller::playNextSong);
    connect(view->btn_previous, &QPushButton::clicked, this, &Controller::playPreviousSong);
    connect(view->volume_slider, &QSlider::valueChanged, this, &Controller::updateVolume);
    connect(view->skip_ad_button, &QPushButton::clicked, this, &Controller::skipAd);
    connect(view->btn_add_song, &QPushButton::clicked, this, &Controller::addSong);
    connect(view->btn_remove_song, &QPushButton::clicked, this, &Controller::removeSong);
    connect(view->btn_sort_by_number, &QPushButton::clicked, this, &Controller::sortByNumber);
    connect(view->btn_sort_by_name, &QPushButton::clicked, this, &Controller::sortByName);
    connect(model, &Model::songs_updated, view, &View::update_song_list);
    connect(view, &View::files_dropped, model, &Model::drop_files);
    connect(media_player_, &QMediaPlayer::mediaStatusChanged, this, &Controller::handleMediaStatus);
    connect(model, &Model::disable_buttons, view, [view]() { view->set_buttons_enabled(false); });
}

void Controller::playSelectedSong() {
    if (media_player_->playbackState() == QMediaPlayer::PausedState) {
        media_player_->play();
        return;
    }

    if (QRandomGenerator::global()->bounded(100) < 30) {
        song_stack_.push(view_->song_list_view->currentIndex().row());
        playRandomAd();
        return;
    }

    view_->skip_ad_button->setVisible(false);

    QModelIndex index = view_->song_list_view->currentIndex();
    if (!index.isValid()) return;

    QString filePath = model_->get_song_at(index.row());
    if (filePath.isEmpty()) return;

    media_player_->stop();
    media_player_->setSource(QUrl());
    media_player_->setSource(QUrl::fromLocalFile(filePath));
    media_player_->play();
}

void Controller::playNextSong() {
    int currentIndex = view_->song_list_view->currentIndex().row();
    if (currentIndex < model_->get_songs().size() - 1) {
        QModelIndex nextIndex = view_->song_list_model->index(currentIndex + 1,0);
        view_->song_list_view->setCurrentIndex(nextIndex);
        playSelectedSong();
    } else {
        media_player_->stop();
    }
}

void Controller::playPreviousSong() {
    int currentIndex = view_->song_list_view->currentIndex().row();
    if (currentIndex > 0) {
        QModelIndex prevIndex = view_->song_list_model->index(currentIndex - 1,0);
        view_->song_list_view->setCurrentIndex(prevIndex);
        playSelectedSong();
    }
}

void Controller::addSong() {
    QString filePath = QFileDialog::getOpenFileName(view_);
    if (!filePath.isEmpty()) {
        model_->add(filePath);
        view_->update_song_list(model_->get_songs());
    }
}

void Controller::removeSong() {
    QModelIndex index = view_->song_list_view->currentIndex();
    if (!index.isValid()) return;

    QString filePath = model_->get_song_at(index.row());
    if (filePath.isEmpty()) return;

    model_->remove(filePath);
}

void Controller::playRandomAd() {
    QString adPath = model_->get_random_ad();
    if (adPath.isEmpty()) return;

    qDebug() << "Playing ad:" << adPath;

    model_->set_playing_ad(true);
    view_->set_buttons_enabled(false);
    int delay = QRandomGenerator::global()->bounded(5, 10) * 1000;
    QTimer::singleShot(delay, view_->skip_ad_button, &QPushButton::show);

    media_player_->stop();
    media_player_->setSource(QUrl::fromLocalFile(adPath));
    media_player_->play();
}

void Controller::skipAd() {
    if (!model_->is_playing_ad()) return;

    media_player_->stop();
    model_->set_playing_ad(false);
    view_->skip_ad_button->setVisible(false);
    view_->set_buttons_enabled(true);

    if (!song_stack_.isEmpty()) {
        int originalSongIndex = song_stack_.pop();
        QModelIndex index = view_->song_list_model->index(originalSongIndex,0);
        view_->song_list_view->setCurrentIndex(index);
        playSelectedSong();
    } else {
        playNextSong();
    }
}

void Controller::updateVolume(int volume) {
    qreal volumeFactor = volume / 100.0;
    audio_output_->setVolume(volumeFactor);
    view_->volume_label->setText(QString("Volume: %1%").arg(volume));
}

void Controller::handleMediaStatus(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        if (model_->is_playing_ad()) {
            model_->set_playing_ad(false);
            view_->skip_ad_button->setVisible(false);
            view_->set_buttons_enabled(true);

            if (!song_stack_.isEmpty()) {
                int nextSongIndex = song_stack_.pop();
                QModelIndex index = view_->song_list_model->index(nextSongIndex,0);
                view_->song_list_view->setCurrentIndex(index);
                playSelectedSong();
            } else {
                playNextSong();
            }
            return;
        }

        int nextIndex = view_->song_list_view->currentIndex().row() + 1;
        if (QRandomGenerator::global()->bounded(100) < 30) {
            if (nextIndex < model_->get_songs().size()) {
                song_stack_.push(nextIndex);
            }
            playRandomAd();
        } else {
            if (nextIndex < model_->get_songs().size()) {
                view_->song_list_view->setCurrentIndex(view_->song_list_model->index(nextIndex, 0));
                playSelectedSong();
            } else {
                media_player_->stop();
            }
        }
    }
}

void Controller::sortByNumber() {
    model_->sort_by_number();
    view_->update_song_list(model_->get_songs());
    view_->set_buttons_enabled(true);
}

void Controller::sortByName() {
    model_->sort_by_name();
    view_->update_song_list(model_->get_songs());
    view_->set_buttons_enabled(true);
}

