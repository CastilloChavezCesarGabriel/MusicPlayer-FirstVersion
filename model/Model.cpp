#include "Model.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QCoreApplication>
#include <QStandardPaths>

Model::Model(QObject *parent) : QObject(parent) {
    resourcesPath = QDir::currentPath() + "/resources";
    announcementsPath = QDir::currentPath() + "/Announcements";
    loadSongs();
    loadAdFiles();
    std::shuffle(songs.begin(), songs.end(), std::mt19937(QRandomGenerator::global()->generate()));
}

void Model::loadSongs() {
    QString directionPath = getMusicPath();
    QDir dir(directionPath);

    if (!dir.exists()) {
        qWarning() << "Music directory not found!";
        return;
    }

    songList = getExtension(directionPath);
    songs.clear();

    for (auto &file : songList) {
        QString fullPath = dir.absoluteFilePath(file);
        songs.append(Song(0,file, fullPath));
    }

    shuffle();
    emit songsUpdated(songList);
}

void Model::loadAdFiles() {
    QString adDirectionPath = getAnnouncementsPath();
    QDir adDir(adDirectionPath);

    if (!adDir.exists()) {
        qWarning() << "Announcements directory not found!";
        return;
    }

    adFiles = getExtension(adDirectionPath);

    for (auto &file : adFiles) {
        file = adDir.absoluteFilePath(file);
    }

    if (adFiles.isEmpty()) {
        qWarning() << "No ads found!";
    } else {
        qDebug() << "Loaded ads:" << adFiles;
    }

    emit adsUpdated(adFiles);
}

QStringList Model::getExtension(const QString &directionPath) const {
    QDir dir(directionPath);
    QStringList filters = {"*.mp3", "*.wav"};
    return dir.entryList(filters, QDir::Files);
}

QString Model::getMusicPath() const {
    return QCoreApplication::applicationDirPath() + "/resources/music";
}

QString Model::getAnnouncementsPath() const {
    return QCoreApplication::applicationDirPath() + "/resources/announcements";
}

void Model::add(const QString &filePath) {
    if (filePath.isEmpty()) return;
    saveToResources(filePath);

    QFileInfo info(filePath);
    QString newFilePath = getMusicPath() + "/" + info.fileName();
    songs.append(Song(0, info.fileName(), newFilePath));
    songList.append(newFilePath);

    emit songsUpdated(songList);
}

void Model::remove(const QString &filePath) {
    if (filePath.isEmpty()) return;

    auto foundSong = std::find_if(songs.begin(), songs.end(), [&](Song &song) {
        return song.getFilePath() == filePath;
    });

    if (foundSong != songs.end()) {
        if (QFile::remove(filePath)) {
            qDebug() << "Deleted song file:" << filePath;
        } else {
            qWarning() << "Failed to delete song file:" << filePath;
        }

        songs.erase(foundSong);
        updateList();
    } else {
        qWarning() << "Song not found in list.";
    }
}

void Model::saveToResources(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    QString destination = getMusicPath();
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

QString Model::getRandomAd() const {
    if (adFiles.isEmpty()) return {};
    return adFiles.at(QRandomGenerator::global()->bounded(adFiles.size()));
}

void Model::dropFiles(const QList<QUrl> &urls) {
    QStringList newSongs;

    for (const QUrl &url : urls) {
        QString filePath = url.toLocalFile();
        if (filePath.endsWith(".mp3") || filePath.endsWith(".wav")) {
            newSongs.append(filePath);
            add(filePath);
        }
    }

    if (!newSongs.isEmpty()) {
        songList.append(newSongs);
        emit songsUpdated(songList);
        emit disableButtons();
    }
}

void Model::sortByNumber() {
    sort.shellSort(songs);
    lastSortMethod = "number";
    updateList();
}

void Model::sortByName() {
    sort.quickSort(songs, 0, songs.size() - 1);
    lastSortMethod = "name";
    updateList();
}

QString Model::getSongAt(int index) const {
    if (index < 0 || index >= songs.size()) return QString();
    return songs[index].getFilePath();
}

void Model::shuffle() {
    std::shuffle(songs.begin(), songs.end(), std::mt19937(QRandomGenerator::global()->generate()));
}

void Model::updateList() {
    songList.clear();
    for (const Song &song : songs) {
        songList.append(song.getFilePath());
    }
    emit songsUpdated(songList);
}

