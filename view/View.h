#ifndef MUSIC_PLAYER_IMPROVED_VIEW_H
#define MUSIC_PLAYER_IMPROVED_VIEW_H
#include <QWidget>
#include <QPushButton>
#include <QListView>
#include <QStringListModel>
#include <QHBoxLayout>
#include <QLabel>

class View : public QWidget {
Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    void set_buttons_enabled(bool enabled) const;

    QPushButton *btn_play{};
    QPushButton *btn_pause{};
    QPushButton *btn_stop{};
    QPushButton *btn_next{};
    QPushButton *btn_previous{};
    QPushButton *btn_add_song{};
    QPushButton *btn_remove_song{};
    QPushButton *btn_sort_by_number;
    QPushButton *btn_sort_by_name;
    QPushButton *skip_ad_button{};

    QListView *song_list_view{};
    QStringListModel *song_list_model{};

    QSlider *volume_slider{};
    QLabel *volume_label{};

private:
    QVBoxLayout *main_layout_{};
    QHBoxLayout *control_layout_{};
    QHBoxLayout *volume_layout_{};
    QHBoxLayout *files_layout_{};
    QHBoxLayout *sort_layout_{};
    void setup_ui();

public slots:
    void update_song_list(const QStringList &songs) const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:
    void files_dropped(const QList<QUrl> &urls);
};

#endif //MUSIC_PLAYER_IMPROVED_VIEW_H
