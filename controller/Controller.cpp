#include "Controller.h"
#include <QDebug>
#include <QTimer>

Controller::Controller(Model *model, View *view, QObject *parent)
        : QObject(parent), model(model), view(view) {

    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);

    connect(view->btnPlay, &QPushButton::clicked, this, &Controller::playSelectedSong);
    connect(view->btnPause, &QPushButton::clicked, mediaPlayer, &QMediaPlayer::pause);
    connect(view->btnStop, &QPushButton::clicked, mediaPlayer, &QMediaPlayer::stop);
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
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &Controller::handleMediaStatus);
    connect(model, &Model::disableButtons, view, [view]() { view->setButtonsEnabled(false); });
}

void Controller::playSelectedSong() {
    if (mediaPlayer->playbackState() == QMediaPlayer::PausedState) {
        mediaPlayer->play();
        return;
    }

    if (QRandomGenerator::global()->bounded(100) < 30) {
        songStack.push(view->songListView->currentIndex().row());
        playRandomAd();
        return;
    }

    view->skipAdButton->setVisible(false);

    QModelIndex index = view->songListView->currentIndex();
    if (!index.isValid()) return;

    QString filePath = model->getSongAt(index.row());
    if (filePath.isEmpty()) return;

    mediaPlayer->stop();
    mediaPlayer->setSource(QUrl());
    mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
    mediaPlayer->play();
}

void Controller::playNextSong() {
    int currentIndex = view->songListView->currentIndex().row();
    if (currentIndex < model->getSongs().size() - 1) {
        QModelIndex nextIndex = view->songListModel->index(currentIndex + 1,0);
        view->songListView->setCurrentIndex(nextIndex);
        playSelectedSong();
    } else {
        mediaPlayer->stop();
    }
}

void Controller::playPreviousSong() {
    int currentIndex = view->songListView->currentIndex().row();
    if (currentIndex > 0) {
        QModelIndex prevIndex = view->songListModel->index(currentIndex - 1,0);
        view->songListView->setCurrentIndex(prevIndex);
        playSelectedSong();
    }
}

void Controller::addSong() {
    QString filePath = QFileDialog::getOpenFileName(view);
    if (!filePath.isEmpty()) {
        model->add(filePath);
        view->updateSongList(model->getSongs());
    }
}

void Controller::removeSong() {
    QModelIndex index = view->songListView->currentIndex();
    if (!index.isValid()) return;

    QString filePath = model->getSongAt(index.row());
    if (filePath.isEmpty()) return;

    model->remove(filePath);
}

void Controller::playRandomAd() {
    QString adPath = model->getRandomAd();
    if (adPath.isEmpty()) return;

    qDebug() << "Playing ad:" << adPath;

    model->setPlayingAd(true);
    view->setButtonsEnabled(false);
    int delay = QRandomGenerator::global()->bounded(5, 10) * 1000;
    QTimer::singleShot(delay, view->skipAdButton, &QPushButton::show);

    mediaPlayer->stop();
    mediaPlayer->setSource(QUrl::fromLocalFile(adPath));
    mediaPlayer->play();
}

void Controller::skipAd() {
    if (!model->isPlayingAd()) return;

    mediaPlayer->stop();
    model->setPlayingAd(false);
    view->skipAdButton->setVisible(false);
    view->setButtonsEnabled(true);

    if (!songStack.isEmpty()) {
        int originalSongIndex = songStack.pop();
        QModelIndex index = view->songListModel->index(originalSongIndex,0);
        view->songListView->setCurrentIndex(index);
        playSelectedSong();
    } else {
        playNextSong();
    }
}

void Controller::updateVolume(int volume) {
    qreal volumeFactor = volume / 100.0;
    audioOutput->setVolume(volumeFactor);
    view->volumeLabel->setText(QString("Volume: %1%").arg(volume));
}

void Controller::handleMediaStatus(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        if (model->isPlayingAd()) {
            model->setPlayingAd(false);
            view->skipAdButton->setVisible(false);
            view->setButtonsEnabled(true);

            if (!songStack.isEmpty()) {
                int nextSongIndex = songStack.pop();
                QModelIndex index = view->songListModel->index(nextSongIndex,0);
                view->songListView->setCurrentIndex(index);
                playSelectedSong();
            } else {
                playNextSong();
            }
            return;
        }

        int nextIndex = view->songListView->currentIndex().row() + 1;
        if (QRandomGenerator::global()->bounded(100) < 30) {
            if (nextIndex < model->getSongs().size()) {
                songStack.push(nextIndex);
            }
            playRandomAd();
        } else {
            if (nextIndex < model->getSongs().size()) {
                view->songListView->setCurrentIndex(view->songListModel->index(nextIndex, 0));
                playSelectedSong();
            } else {
                mediaPlayer->stop();
            }
        }
    }
}

void Controller::sortByNumber() {
    model->sortByNumber();
    view->updateSongList(model->getSongs());
    view->setButtonsEnabled(true);
}

void Controller::sortByName() {
    model->sortByName();
    view->updateSongList(model->getSongs());
    view->setButtonsEnabled(true);
}

