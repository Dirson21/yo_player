#ifndef TRACK_H
#define TRACK_H
#include <QObject>
#include <QString>
#include <QVector>
#include <BASS/c/bass.h>
#include <BASS_flac/c/bassflac.h>
#include <tags/c/tags.h>
#include <QTimer>

struct TrackInfo
{
    BASS_CHANNELINFO bassInfo;
    double time;
    int size;
    DWORD bitrate;
    QStringList trackTags;
};

void CALLBACK callbackEnd(HSYNC handle, DWORD channel, DWORD data, void *user);

class Track :public QObject
{
    Q_OBJECT

public:
    Track();
    ~Track();
    void setTrack(QString trackName);
    void setVolume(double volume);
    void play();
    void stop();
    void pause();
    void resume();
    bool isStream();
    int getActive();
    int getStatus();
    double getPositionTime();
    QString getName();
    int GetPositionByte();
    bool isTrackEnd();
    void setPositionTime(double time);
    void setPositionByte(QWORD byte);
    int isPlaying();
    TrackInfo* getTrackInfo();
    void deleteStream();
    void signalEndEx(); //вызывает сигнал trackEnd

signals:
    void trackEnd();
    void trackPlay();
    void trackPause();
    void trackStop();
    void trackPlaying();
    void trackSeted();

private:
    QString trackName;
    HSTREAM trackStream = 0;
    bool repeat = 0;
    TrackInfo trackInfo;
    int trackTime;
    int trackSize;
    HSYNC syncEnd, syncStall;
    double volume = 0.5f;
    QTimer *playingTimer;
    bool endPlay = 0;

    TrackInfo* getInfo(TrackInfo *bufferInfo);
    QStringList getTag();
};

#endif // TRACK_H
