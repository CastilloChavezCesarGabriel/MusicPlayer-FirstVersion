#include "View.h"
#include <QDragEnterEvent>
#include <QMimeData>

View::View(QWidget *parent) : QWidget(parent) {
    setup_ui();
    setAcceptDrops(true);
}

void View::setup_ui() {
    setWindowTitle("Music Player");
    resize(500, 400);

    sortLayout = new QHBoxLayout();
    btnSortByNumber = new QPushButton("Sort by Number", this);
    btnSortByName = new QPushButton("Sort Alphabetically", this);
    sortLayout->addWidget(btnSortByNumber);
    sortLayout->addWidget(btnSortByName);
    sortLayout->setAlignment(Qt::AlignCenter);

    mainLayout = new QVBoxLayout(this);
    controlLayout = new QHBoxLayout();
    volumeLayout = new QHBoxLayout();
    filesLayout = new QHBoxLayout();

    songListView = new QListView(this);
    songListModel = new QStringListModel(this);
    songListView->setModel(songListModel);

    btnPlay = new QPushButton("▶", this);
    btnPause = new QPushButton("⏸", this);
    btnStop = new QPushButton("⏹", this);
    btnNext = new QPushButton("⏭", this);
    btnPrevious = new QPushButton("⏮", this);
    btnAddSong = new QPushButton("✚ Add Song", this);
    btnRemoveSong = new QPushButton("⚪ Remove Song", this);
    skipAdButton = new QPushButton("⏭ Skip Ad", this);
    skipAdButton->setVisible(false);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);

    volumeLabel = new QLabel("Volume: 50%", this);

    controlLayout->addWidget(btnPrevious);
    controlLayout->addWidget(btnPlay);
    controlLayout->addWidget(btnPause);
    controlLayout->addWidget(btnStop);
    controlLayout->addWidget(btnNext);
    controlLayout->setAlignment(Qt::AlignCenter);

    volumeLayout->addWidget(volumeLabel);
    volumeLayout->addWidget(volumeSlider);
    volumeLayout->setAlignment(Qt::AlignCenter);

    filesLayout->addWidget(btnAddSong);
    filesLayout->addWidget(btnRemoveSong);
    filesLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(sortLayout);
    mainLayout->addWidget(songListView);
    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(volumeLayout);
    mainLayout->addWidget(skipAdButton, 0, Qt::AlignCenter);
    mainLayout->addLayout(filesLayout);

    setLayout(mainLayout);
}

void View::setButtonsEnabled(bool enabled) const {
    btnPlay->setEnabled(enabled);
    btnPause->setEnabled(enabled);
    btnStop->setEnabled(enabled);
    btnNext->setEnabled(enabled);
    btnPrevious->setEnabled(enabled);
    btnAddSong->setEnabled(enabled);
    btnRemoveSong->setEnabled(enabled);
}

void View::updateSongList(const QStringList &songs) const {
    songListModel->setStringList(songs);
}

void View::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void View::dropEvent(QDropEvent *event){
    QList<QUrl> urls = event->mimeData()->urls();
    if (!urls.isEmpty()) {
        emit filesDropped(urls);
    }
}


