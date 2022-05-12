#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "music_class/track.h"

int main(int argc, char *argv[])
{
    BASS_Init(-1, 44100, 0, 0, NULL);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
