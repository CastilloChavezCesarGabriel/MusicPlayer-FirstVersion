#ifndef MUSIC_PLAYER_IMPROVED_VIEW_H
#define MUSIC_PLAYER_IMPROVED_VIEW_H
#include <QWidget>
#include <QPushButton>
#include <QListView>
#include <QStringListModel>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>

class View : public QWidget {
Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    void setButtonsEnabled(bool enabled) const;

    QPushButton *btnPlay{};
    QPushButton *btnPause{};
    QPushButton *btnStop{};
    QPushButton *btnNext{};
    QPushButton *btnPrevious{};
    QPushButton *btnAddSong{};
    QPushButton *btnRemoveSong{};
    QPushButton *btnSortByNumber;
    QPushButton *btnSortByName;
    QPushButton *skipAdButton{};

    QListView *songListView{};
    QStringListModel *songListModel{};

    QSlider *volumeSlider{};
    QLabel *volumeLabel{};

private:
    QVBoxLayout *mainLayout{};
    QHBoxLayout *controlLayout{};
    QHBoxLayout *volumeLayout{};
    QHBoxLayout *filesLayout{};
    QHBoxLayout *sortLayout{};
    void setupUI();

public slots:
    void updateSongList(const QStringList &songs) const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:
    void filesDropped(const QList<QUrl> &urls);
};

#endif //MUSIC_PLAYER_IMPROVED_VIEW_H
