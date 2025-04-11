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
    QString direction_path = get_music_path();
    QDir dir(direction_path);

    if (!dir.exists()) {
        qWarning() << "Music directory not found!";
        return;
    }

    song_list_ = get_extension(direction_path);
    songs_.clear();

    for (auto &file : song_list_) {
        QString full_path = dir.absoluteFilePath(file);
        songs_.append(Song(0,file, full_path));
    }

    shuffle();
    emit songs_updated(song_list_);
}

void Model::load_ad_files() {
    QString ad_direction_path = get_announcements_path();
    QDir adDir(ad_direction_path);

    if (!adDir.exists()) {
        qWarning() << "Announcements directory not found!";
        return;
    }

    ad_files_ = get_extension(ad_direction_path);

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

QStringList Model::get_extension(const QString &direction_path) {
    QDir dir(direction_path);
    QStringList filters = {"*.mp3", "*.wav"};
    return dir.entryList(filters, QDir::Files);
}

QString Model::get_music_path() {
    return QCoreApplication::applicationDirPath() + "/resources/music";
}

QString Model::get_announcements_path() {
    return QCoreApplication::applicationDirPath() + "/resources/announcements";
}

void Model::add(const QString &file_path) {
    if (file_path.isEmpty()) return;
    save_to_resources(file_path);

    QFileInfo info(file_path);
    QString new_file_path = get_music_path() + "/" + info.fileName();
    songs_.append(Song(0, info.fileName(), new_file_path));
    song_list_.append(new_file_path);

    emit songs_updated(song_list_);
}

void Model::remove(const QString &file_path) {
    if (file_path.isEmpty()) return;

    auto found_song = std::find_if(songs_.begin(), songs_.end(), [&](Song &song) {
        return song.get_file_path() == file_path;
    });

    if (found_song != songs_.end()) {
        if (QFile::remove(file_path)) {
            qDebug() << "Deleted song file:" << file_path;
        } else {
            qWarning() << "Failed to delete song file:" << file_path;
        }

        songs_.erase(found_song);
        update_list();
    } else {
        qWarning() << "Song not found in list.";
    }
}

void Model::save_to_resources(const QString &file_path) {
    QFileInfo file_info(file_path);
    QString destination = get_music_path();
    QString destination_path = destination + "/" + file_info.fileName();

    if (QFile::exists(destination_path)) {
        qWarning() << "File already exists in resources.";
        return;
    }

    if (QFile::copy(file_path, destination_path)) {
        qDebug() << "File copied to resources:" << destination_path;
    } else {
        qWarning() << "Failed to copy file to resources.";
    }
}

QString Model::get_random_ad() const {
    if (ad_files_.isEmpty()) return {};
    return ad_files_.at(QRandomGenerator::global()->bounded(ad_files_.size()));
}

void Model::drop_files(const QList<QUrl> &urls) {
    QStringList new_songs;

    for (const QUrl &url : urls) {
        QString file_path = url.toLocalFile();
        if (file_path.endsWith(".mp3") || file_path.endsWith(".wav")) {
            new_songs.append(file_path);
            add(file_path);
        }
    }

    if (!new_songs.isEmpty()) {
        song_list_.append(new_songs);
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

