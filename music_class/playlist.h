#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <QObject>
#include <QString>
#include <QVector>
#include "track.h"
class PlayList : public QObject
{
    Q_OBJECT
public:
    PlayList();
    ~PlayList();
    void addTrack(QString TrackName);
    void changeTrack(int pos);
    void playPos(int pos);
    void play();
    void pause();
    void stop();
    int getCurrentPos();
    int getCountTracks();
    Track* getCurrentTrack();
    void setRepeat(bool repeat);
    void deleteTrack(int pos=0);

public slots:
    void next();
    void prev();

signals:
    void playlistEnd();
    void trackPosChange(int oldPos, int newPos);
    void trackDeleted();
    void tracksNameEnd();

private:
    QVector<QString> tracksName;
    Track *currentTrack;
    int currentPos = -1;
    bool repeat = 0;
    bool ispause = 0;
    bool isTrackEnd = 0;

    int nextPos(int pos);
    int prevPos(int pos);
};
#endif // PLAYLIST_H
