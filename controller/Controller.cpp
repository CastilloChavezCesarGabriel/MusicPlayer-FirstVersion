#include "Controller.h"
#include <QTimer>
#include <QRandomGenerator>

Controller::Controller(Model *model, View *view, QObject *parent)
        : QObject(parent), model_(model), view_(view) {

    media_player_ = new QMediaPlayer(this);
    audio_output_ = new QAudioOutput(this);
    media_player_->setAudioOutput(audio_output_);

    connect(view->btnPlay, &QPushButton::clicked, this, &Controller::playSelectedSong);
    connect(view->btnPause, &QPushButton::clicked, media_player_, &QMediaPlayer::pause);
    connect(view->btnStop, &QPushButton::clicked, media_player_, &QMediaPlayer::stop);
    connect(view->btnNext, &QPushButton::clicked, this, &Controller::playNextSong);
    connect(view->btnPrevious, &QPushButton::clicked, this, &Controller::playPreviousSong);
    connect(view->volumeSlider, &QSlider::valueChanged, this, &Controller::updateVolume);
    connect(view->skipAdButton, &QPushButton::clicked, this, &Controller::skipAd);
    connect(view->btnAddSong, &QPushButton::clicked, this, &Controller::addSong);
    connect(view->btnRemoveSong, &QPushButton::clicked, this, &Controller::removeSong);
    connect(view->btnSortByNumber, &QPushButton::clicked, this, &Controller::sortByNumber);
    connect(view->btnSortByName, &QPushButton::clicked, this, &Controller::sortByName);
    connect(model, &Model::songsUpdated, view, &View::updateSongList);
    connect(view, &View::filesDropped, model, &Model::dropFiles);
    connect(media_player_, &QMediaPlayer::mediaStatusChanged, this, &Controller::handleMediaStatus);
    connect(model, &Model::disableButtons, view, [view]() { view->setButtonsEnabled(false); });
}

void Controller::playSelectedSong() {
    if (media_player_->playbackState() == QMediaPlayer::PausedState) {
        media_player_->play();
        return;
    }

    if (QRandomGenerator::global()->bounded(100) < 30) {
        song_stack_.push(view_->songListView->currentIndex().row());
        playRandomAd();
        return;
    }

    view_->skipAdButton->setVisible(false);

    QModelIndex index = view_->songListView->currentIndex();
    if (!index.isValid()) return;

    QString filePath = model_->getSongAt(index.row());
    if (filePath.isEmpty()) return;

    media_player_->stop();
    media_player_->setSource(QUrl());
    media_player_->setSource(QUrl::fromLocalFile(filePath));
    media_player_->play();
}

void Controller::playNextSong() {
    int currentIndex = view_->songListView->currentIndex().row();
    if (currentIndex < model_->getSongs().size() - 1) {
        QModelIndex nextIndex = view_->songListModel->index(currentIndex + 1,0);
        view_->songListView->setCurrentIndex(nextIndex);
        playSelectedSong();
    } else {
        media_player_->stop();
    }
}

void Controller::playPreviousSong() {
    int currentIndex = view_->songListView->currentIndex().row();
    if (currentIndex > 0) {
        QModelIndex prevIndex = view_->songListModel->index(currentIndex - 1,0);
        view_->songListView->setCurrentIndex(prevIndex);
        playSelectedSong();
    }
}

void Controller::addSong() {
    QString filePath = QFileDialog::getOpenFileName(view_);
    if (!filePath.isEmpty()) {
        model_->add(filePath);
        view_->updateSongList(model_->getSongs());
    }
}

void Controller::removeSong() {
    QModelIndex index = view_->songListView->currentIndex();
    if (!index.isValid()) return;

    QString filePath = model_->getSongAt(index.row());
    if (filePath.isEmpty()) return;

    model_->remove(filePath);
}

void Controller::playRandomAd() {
    QString adPath = model_->getRandomAd();
    if (adPath.isEmpty()) return;

    qDebug() << "Playing ad:" << adPath;

    model_->setPlayingAd(true);
    view_->setButtonsEnabled(false);
    int delay = QRandomGenerator::global()->bounded(5, 10) * 1000;
    QTimer::singleShot(delay, view_->skipAdButton, &QPushButton::show);

    media_player_->stop();
    media_player_->setSource(QUrl::fromLocalFile(adPath));
    media_player_->play();
}

void Controller::skipAd() {
    if (!model_->isPlayingAd()) return;

    media_player_->stop();
    model_->setPlayingAd(false);
    view_->skipAdButton->setVisible(false);
    view_->setButtonsEnabled(true);

    if (!song_stack_.isEmpty()) {
        int originalSongIndex = song_stack_.pop();
        QModelIndex index = view_->songListModel->index(originalSongIndex,0);
        view_->songListView->setCurrentIndex(index);
        playSelectedSong();
    } else {
        playNextSong();
    }
}

void Controller::updateVolume(int volume) {
    qreal volumeFactor = volume / 100.0;
    audio_output_->setVolume(volumeFactor);
    view_->volumeLabel->setText(QString("Volume: %1%").arg(volume));
}

void Controller::handleMediaStatus(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        if (model_->isPlayingAd()) {
            model_->setPlayingAd(false);
            view_->skipAdButton->setVisible(false);
            view_->setButtonsEnabled(true);

            if (!song_stack_.isEmpty()) {
                int nextSongIndex = song_stack_.pop();
                QModelIndex index = view_->songListModel->index(nextSongIndex,0);
                view_->songListView->setCurrentIndex(index);
                playSelectedSong();
            } else {
                playNextSong();
            }
            return;
        }

        int nextIndex = view_->songListView->currentIndex().row() + 1;
        if (QRandomGenerator::global()->bounded(100) < 30) {
            if (nextIndex < model_->getSongs().size()) {
                song_stack_.push(nextIndex);
            }
            playRandomAd();
        } else {
            if (nextIndex < model_->getSongs().size()) {
                view_->songListView->setCurrentIndex(view_->songListModel->index(nextIndex, 0));
                playSelectedSong();
            } else {
                media_player_->stop();
            }
        }
    }
}

void Controller::sortByNumber() {
    model_->sortByNumber();
    view_->updateSongList(model_->getSongs());
    view_->setButtonsEnabled(true);
}

void Controller::sortByName() {
    model_->sortByName();
    view_->updateSongList(model_->getSongs());
    view_->setButtonsEnabled(true);
}

