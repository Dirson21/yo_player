#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QTableView>
#include <QSettings>
#include <QFile>
#include <fstream>
#include <QProcess>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    playlist = new PlayList();
    currentTrack = playlist->getCurrentTrack();
    currentTrack->setVolume((double)ui->VolumeSlider->value() / 100);
    playlist->setRepeat(0);

    connect(playlist, &PlayList::playlistEnd, [this]() {
        setWidgetPlay(0);
    });

    //изменение кнопки играть/пауза
    connect(currentTrack, SIGNAL(trackPlay()), SLOT(changeWidgetPlay()));
    connect(currentTrack, SIGNAL(trackPause()), SLOT(changeWidgetPlay()));
    connect(currentTrack, SIGNAL(trackStop()), SLOT(changeWidgetPlay()));
    connect(playlist, SIGNAL(trackDeleted()), SLOT(changeWidgetPlay()));

    //изменение визуального положения трека
    connect(playlist, &PlayList::trackPosChange, [this](int oldPos, int newPos)
    {
        if (oldPos >= 0)
            ui->PlaylistWidget->item(oldPos)->setBackground(Qt::white);
        if (newPos >= 0)
            ui->PlaylistWidget->item(newPos)->setBackground(Qt::gray);
        //выводим информацию о треке
        TrackInfo* trackInfo = currentTrack->getTrackInfo();
        QString time;
        int min, sec;

        min = (int)trackInfo->time / 60;
        sec = (int)trackInfo->time % 60;

        time = QString::number(min / 10) + QString::number(min % 10);
        time += ":";
        time += QString::number(sec / 10) + QString::number(sec % 10);

        ui->AudioNameLabel->setText(currentTrack->getName());
        ui->nameTagLabbel->setText(trackInfo->trackTags[0]);
        ui->ispolTagLabel->setText(trackInfo->trackTags[1]);
        ui->albumTagLabel->setText(trackInfo->trackTags[2]);
        ui->timeTrackLabel->setText(time);
        ui->bitrateLabel->setText(QString::number(trackInfo->bitrate) + " kb/s");
        if (ui->PlaylistWidget->currentItem())
        {
            ui->PlaylistWidget->currentItem()->setSelected(0);
        }

    });

    //изменение громкости
    connect(ui->VolumeSlider, &QSlider::valueChanged, [this](int value)
    {
        currentTrack->setVolume((double)value/100);
        choiseVolumeIcon(value);

    });

    connect(currentTrack, SIGNAL(trackSeted()), SLOT(sliderSetTrack()));
    connect(currentTrack, SIGNAL(trackStop()), SLOT(sliderSetTrack()));
    connect(playlist, &PlayList::tracksNameEnd, [this]()
    {
        ui->timerTrackSlider->setValue(0);
        ui->AudioNameLabel->setText("");
        ui->nameTagLabbel->setText("");
        ui->ispolTagLabel->setText("");
        ui->albumTagLabel->setText("");
        ui->timeTrackLabel->setText("");
        ui->bitrateLabel->setText("");

    });

    //изменение слайдера трека
    connect(currentTrack, &Track::trackPlaying, [this]()
    {
        //ui->timerTrackSlider->setValue(currentTrack->getPositionTime() * 10000);
        ui->timerTrackSlider->setValue(currentTrack->GetPositionByte());
        // ui->timerTrackSlider->setTickInterval(currentTrack->getPositionTime());
    });


    connect(ui->PlaylistWidget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item)
    {
        playlist->changeTrack(ui->PlaylistWidget->currentRow());
    });



    connect(ui->timerTrackSlider, &QSlider::sliderPressed, [this]()
    {
        qDebug() << ui->timerTrackSlider->value();
        currentTrack->pause();

        qDebug() <<"press";
    });

    connect(ui->timerTrackSlider, &QSlider::sliderReleased, [this]()
    {
        //currentTrack->setPositionTime(ui->timerTrackSlider->value() / 10000);
        qDebug() << ui->timerTrackSlider->value();
        currentTrack->setPositionByte(ui->timerTrackSlider->value());
        currentTrack->play();
    });
    save_load();
}

MainWindow::~MainWindow()
{
    save();
    delete playlist;
    delete ui;
}


void MainWindow::on_AddButton_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      tr("Open files"),
                                                      QString(),
                                                      tr("Audio Files (*.mp3 *.flac *wav)"));
    if (files.size())
    {
        foreach(QString filePath, files)
        {
            ui->PlaylistWidget->addItem(filePath);
            playlist->addTrack(filePath);
        }
        //ui->PlaylistWidget->item(0)->setBackground(Qt::gray);

    }
}

void MainWindow::on_NextButton_clicked()
{
    playlist->next();
}

void MainWindow::on_PrevButton_clicked()
{
    playlist->prev();
}


void MainWindow::on_PlayButton_clicked()
{
    if (ui->PlayButton->text() == "play")
    {
        playlist->play();
    }
    else
    {
        playlist->pause();
    }
}

void MainWindow::setWidgetPlay(int isplay)
{
    if (isplay == BASS_ACTIVE_PLAYING)
    {
        ui->PlayButton->setText("pause");
        ui->PlayButton->setIcon(QIcon(":/img/ico/pause.png"));
    }
    else
    {
        ui->PlayButton->setText("play");
        ui->PlayButton->setIcon(QIcon(":/img/ico/play.png"));
    }
}


void MainWindow::changeWidgetPlay()
{
    setWidgetPlay(currentTrack->isPlaying());
}


void MainWindow::on_VolumeButton_clicked()
{
    if (ui->VolumeButton->text() == "volume")
    {
        choiseVolumeIcon(0);
        currentTrack->setVolume(0);
    }
    else
    {
        choiseVolumeIcon(ui->VolumeSlider->value());
        currentTrack->setVolume((double)ui->VolumeSlider->value() / 100);
    }
}


void MainWindow::choiseVolumeIcon(int value)
{
    if (value > 0 && value <= 33)
    {
        ui->VolumeButton->setIcon(QIcon(":/img/ico/media-volume-1-32.png"));
    }
    else if (value > 33 && value <= 66)
    {
        ui->VolumeButton->setIcon(QIcon(":/img/ico/media-volume-2-32.png"));
    }
    else if (value > 66)
    {
        ui->VolumeButton->setIcon(QIcon(":/img/ico/media-volume-3-32.png"));
    }
    else
    {
        ui->VolumeButton->setIcon(QIcon(":/img/ico/media-mute-32.png"));
    }
    if (value)
    {
        ui->VolumeButton->setText("volume");
    }
    else
    {
        ui->VolumeButton->setText("mute");
    }

}

void MainWindow::on_stopMusicButton_clicked()
{
    currentTrack->stop();
}

void MainWindow::sliderSetTrack()
{
    TrackInfo *trackInfo = currentTrack->getTrackInfo();
    ui->timerTrackSlider->setValue(0);
    ui->timerTrackSlider->setMaximum(trackInfo->size);
}

void MainWindow::updatePlaylistWidget()
{

}

void MainWindow::removePlaylistWidget(int pos)
{
    if (pos < 0) pos = 0;
    ui->PlaylistWidget->removeItemWidget(ui->PlaylistWidget->takeItem(pos));

}

void MainWindow::on_deleteTrackButton_clicked()
{
    int pos = playlist->getCurrentPos();
    removePlaylistWidget(pos);
    playlist->deleteTrack(pos);
}

void MainWindow::save()
{
    QFile file("playlist");
    QTextStream out(&file);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text));
    {
        for(int i = 0; i < ui->PlaylistWidget->count(); i++){
            out << ui->PlaylistWidget->item(i)->text() << "\n";
        }
    }

}

void MainWindow::save_load()
{
    QFile file("playlist");
    QTextStream in(&file);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (QFile::exists(line))
            {
                ui->PlaylistWidget->addItem(line);
                playlist->addTrack(line);
            }
        }
    }
}
