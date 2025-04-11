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

    sort_layout_ = new QHBoxLayout();
    btn_sort_by_number = new QPushButton("Sort by Number", this);
    btn_sort_by_name = new QPushButton("Sort Alphabetically", this);
    sort_layout_->addWidget(btn_sort_by_number);
    sort_layout_->addWidget(btn_sort_by_name);
    sort_layout_->setAlignment(Qt::AlignCenter);

    main_layout_ = new QVBoxLayout(this);
    control_layout_ = new QHBoxLayout();
    volume_layout_ = new QHBoxLayout();
    files_layout_ = new QHBoxLayout();

    song_list_view = new QListView(this);
    song_list_model = new QStringListModel(this);
    song_list_view->setModel(song_list_model);

    btn_play = new QPushButton("▶", this);
    btn_pause = new QPushButton("⏸", this);
    btn_stop = new QPushButton("⏹", this);
    btn_next = new QPushButton("⏭", this);
    btn_previous = new QPushButton("⏮", this);
    btn_add_song = new QPushButton("✚ Add Song", this);
    btn_remove_song = new QPushButton("⚪ Remove Song", this);
    skip_ad_button = new QPushButton("⏭ Skip Ad", this);
    skip_ad_button->setVisible(false);

    volume_slider = new QSlider(Qt::Horizontal, this);
    volume_slider->setRange(0, 100);
    volume_slider->setValue(50);

    volume_label = new QLabel("Volume: 50%", this);

    control_layout_->addWidget(btn_previous);
    control_layout_->addWidget(btn_play);
    control_layout_->addWidget(btn_pause);
    control_layout_->addWidget(btn_stop);
    control_layout_->addWidget(btn_next);
    control_layout_->setAlignment(Qt::AlignCenter);

    volume_layout_->addWidget(volume_label);
    volume_layout_->addWidget(volume_slider);
    volume_layout_->setAlignment(Qt::AlignCenter);

    files_layout_->addWidget(btn_add_song);
    files_layout_->addWidget(btn_remove_song);
    files_layout_->setAlignment(Qt::AlignCenter);

    main_layout_->addLayout(sort_layout_);
    main_layout_->addWidget(song_list_view);
    main_layout_->addLayout(control_layout_);
    main_layout_->addLayout(volume_layout_);
    main_layout_->addWidget(skip_ad_button, 0, Qt::AlignCenter);
    main_layout_->addLayout(files_layout_);

    setLayout(main_layout_);
}

void View::set_buttons_enabled(bool enabled) const {
    btn_play->setEnabled(enabled);
    btn_pause->setEnabled(enabled);
    btn_stop->setEnabled(enabled);
    btn_next->setEnabled(enabled);
    btn_previous->setEnabled(enabled);
    btn_add_song->setEnabled(enabled);
    btn_remove_song->setEnabled(enabled);
}

void View::update_song_list(const QStringList &songs) const {
    song_list_model->setStringList(songs);
}

void View::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void View::dropEvent(QDropEvent *event){
    if (const QList<QUrl> urls = event->mimeData()->urls(); !urls.isEmpty()) {
        emit files_dropped(urls);
    }
}


