#include "Model.h"
#include <QRandomGenerator>
#include <QVector>
#include <QCoreApplication>
#include <QStandardPaths>

Model::Model(QObject *parent) : QObject(parent) {
    resources_path_ = QDir::currentPath() + "/resources";
    announcements_path_ = QDir::currentPath() + "/Announcements";
    load_songs();
    load_ad_files();
    std::shuffle(songs_.begin(), songs_.end(), std::mt19937(QRandomGenerator::global()->generate()));
}

void Model::load_songs() {
    QString directionPath = get_music_path();
    QDir dir(directionPath);

    if (!dir.exists()) {
        qWarning() << "Music directory not found!";
        return;
    }

    song_list_ = get_extension(directionPath);
    songs_.clear();

    for (auto &file : song_list_) {
        QString fullPath = dir.absoluteFilePath(file);
        songs_.append(Song(0,file, fullPath));
    }

    shuffle();
    emit songs_updated(song_list_);
}

void Model::load_ad_files() {
    QString adDirectionPath = get_announcements_path();
    QDir adDir(adDirectionPath);

    if (!adDir.exists()) {
        qWarning() << "Announcements directory not found!";
        return;
    }

    ad_files_ = get_extension(adDirectionPath);

    for (auto &file : ad_files_) {
        file = adDir.absoluteFilePath(file);
    }

    if (ad_files_.isEmpty()) {
        qWarning() << "No ads found!";
    } else {
        qDebug() << "Loaded ads:" << ad_files_;
    }

    emit ads_updated(ad_files_);
}

QStringList Model::get_extension(const QString &directionPath) {
    QDir dir(directionPath);
    QStringList filters = {"*.mp3", "*.wav"};
    return dir.entryList(filters, QDir::Files);
}

QString Model::get_music_path() {
    return QCoreApplication::applicationDirPath() + "/resources/music";
}

QString Model::get_announcements_path() {
    return QCoreApplication::applicationDirPath() + "/resources/announcements";
}

void Model::add(const QString &filePath) {
    if (filePath.isEmpty()) return;
    save_to_resources(filePath);

    QFileInfo info(filePath);
    QString newFilePath = get_music_path() + "/" + info.fileName();
    songs_.append(Song(0, info.fileName(), newFilePath));
    song_list_.append(newFilePath);

    emit songs_updated(song_list_);
}

void Model::remove(const QString &filePath) {
    if (filePath.isEmpty()) return;

    auto found_song = std::find_if(songs_.begin(), songs_.end(), [&](Song &song) {
        return song.get_file_path() == filePath;
    });

    if (found_song != songs_.end()) {
        if (QFile::remove(filePath)) {
            qDebug() << "Deleted song file:" << filePath;
        } else {
            qWarning() << "Failed to delete song file:" << filePath;
        }

        songs_.erase(found_song);
        update_list();
    } else {
        qWarning() << "Song not found in list.";
    }
}

void Model::save_to_resources(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    QString destination = get_music_path();
    QString destinationPath = destination + "/" + fileInfo.fileName();

    if (QFile::exists(destinationPath)) {
        qWarning() << "File already exists in resources.";
        return;
    }

    if (QFile::copy(filePath, destinationPath)) {
        qDebug() << "File copied to resources:" << destinationPath;
    } else {
        qWarning() << "Failed to copy file to resources.";
    }
}

QString Model::get_random_ad() const {
    if (ad_files_.isEmpty()) return {};
    return ad_files_.at(QRandomGenerator::global()->bounded(ad_files_.size()));
}

void Model::drop_files(const QList<QUrl> &urls) {
    QStringList newSongs;

    for (const QUrl &url : urls) {
        QString filePath = url.toLocalFile();
        if (filePath.endsWith(".mp3") || filePath.endsWith(".wav")) {
            newSongs.append(filePath);
            add(filePath);
        }
    }

    if (!newSongs.isEmpty()) {
        song_list_.append(newSongs);
        emit songs_updated(song_list_);
        emit disable_buttons();
    }
}

void Model::sort_by_number() {
    sort_.shell_sort(songs_);
    last_sort_method_ = "number";
    update_list();
}

void Model::sort_by_name() {
    sort_.quick_sort(songs_, 0, songs_.size() - 1);
    last_sort_method_ = "name";
    update_list();
}

QString Model::get_song_at(int index) const {
    if (index < 0 || index >= songs_.size()) return QString();
    return songs_[index].get_file_path();
}

void Model::shuffle() {
    std::shuffle(songs_.begin(), songs_.end(), std::mt19937(QRandomGenerator::global()->generate()));
}

void Model::update_list() {
    song_list_.clear();
    for (const Song &song : songs_) {
        song_list_.append(song.get_file_path());
    }
    emit songs_updated(song_list_);
}

