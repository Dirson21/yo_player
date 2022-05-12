#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <BASS/c/bass.h>
#include <BASS_flac/c/bassflac.h>
#include "music_class/playlist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AddButton_clicked();
    void on_NextButton_clicked();
    void on_PlayButton_clicked();
    void changeWidgetPlay();
    void on_PrevButton_clicked();
    void on_VolumeButton_clicked();
    void on_stopMusicButton_clicked();
    void sliderSetTrack();
    void updatePlaylistWidget();
    void removePlaylistWidget(int pos);
    void on_deleteTrackButton_clicked();
    void save();
    void save_load();

private:
    PlayList *playlist;
    Track* currentTrack;
    Ui::MainWindow *ui;
    int prev_volume = 0;

    void setWidgetPlay(int isplay);
    void choiseVolumeIcon(int value);
};
#endif // MAINWINDOW_H
