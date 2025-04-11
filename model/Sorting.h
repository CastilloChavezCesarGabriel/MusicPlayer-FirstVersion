#ifndef MUSIC_PLAYER_IMPROVED_SORT_METHOD_H
#define MUSIC_PLAYER_IMPROVED_SORT_METHOD_H
#include <QVector>
#include "Song.h"

class Sorting{
public:
    Sorting();
    void shellSort(QVector<Song>& song);
    void quickSort(QVector<Song>& song, int left, int right);

private:
    static int partition(QVector<Song>& song, int left, int right);
};

#endif //MUSIC_PLAYER_IMPROVED_SORT_METHOD_H
