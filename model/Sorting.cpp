#include "Sorting.h"

Sorting::Sorting() = default;

void Sorting::shell_sort(QVector<Song>& song) {
    int n = song.size();
    for (int interval = n / 2; interval > 0; interval /= 2) {
        for (int i = interval; i < n; i++) {
            Song swapValue = song[i];
            int j;
            for (j = i; j >= interval && song[j - interval].getNumber() < swapValue.getNumber(); j -= interval) {
                song[j] = song[j - interval];
            }
            song[j] = swapValue;
        }
    }
}

void Sorting::quick_sort(QVector<Song>& song, int left, int right) {
    if (left < right) {
        int pivotIndex = partition(song, left, right);
        quick_sort(song, left, pivotIndex - 1);
        quick_sort(song, pivotIndex + 1, right);
    }
}

int Sorting::partition(QVector<Song>& song, int left, int right) {
    QString pivot = song[right].getName();
    int i = left - 1;
    for (int j = left; j < right; j++) {
        QString songName = song[j].getName().toLower();
        QString pivotName = pivot.toLower();

        if (songName < pivotName) {
            i++;
            std::swap(song[i], song[j]);
        }
    }
    std::swap(song[i + 1], song[right]);
    return i + 1;
}
