#include "track.h"
#include <QtDebug>
#include <QMimeDatabase>
#include <QFile>
Track::Track()
{
    playingTimer = new QTimer();
    playingTimer->start(1);

    connect(playingTimer, &QTimer::timeout, [this] ()
    {
        if (trackStream && isPlaying() == BASS_ACTIVE_PLAYING)
        {
            emit trackPlaying();
        }
    });

    connect(this, &Track::trackEnd, [this]()
    {
        endPlay = 1;

    });
    connect(this, &Track::trackPlay, [this]()
    {
        endPlay = 0;
    });
}

Track::~Track()
{
    BASS_ChannelStop(trackStream);
    BASS_StreamFree(trackStream);
    delete playingTimer;
}

void Track::setTrack(QString trackName)
{
    if (trackStream)
    {
        BASS_StreamFree(trackStream);
    }
    QMimeDatabase db;
    QMimeType musicFile =  db.mimeTypeForFile(trackName, QMimeDatabase::MatchContent);
    QString type = musicFile.preferredSuffix();
    if (type == "flac")
    {
        trackStream = BASS_FLAC_StreamCreateFile(false, trackName.toStdWString().c_str(), 0, 0, 0);
    }
    else
    {
        trackStream = BASS_StreamCreateFile(false, trackName.toStdWString().c_str(), 0, 0, 0);
    }
    setVolume(volume);
    this->trackName = trackName;
    getInfo(&trackInfo);
    if (trackStream)
    {
        syncEnd = BASS_ChannelSetSync(trackStream, BASS_SYNC_END, 0, callbackEnd , this);
        emit trackSeted();
        qDebug() << "Track seted";
    }
}

void Track::play()
{
    if (BASS_ChannelPlay(trackStream, repeat))
    {
        emit trackPlay();
        qDebug() << "track play";
    }
}

void Track::pause()
{
    if (BASS_ChannelPause(trackStream))
    {
        emit trackPause();
        qDebug() << "track pause";
        qDebug() << BASS_ErrorGetCode();
    }
}

void Track::stop()
{
    if (BASS_ChannelStop(trackStream))
    {
        BASS_ChannelSetPosition(trackStream, 0,BASS_POS_BYTE);
        emit trackStop();
        qDebug() << "track stop";
    }
}

int Track::getStatus()
{
    return BASS_ErrorGetCode();
}

bool Track::isStream()
{
    return (bool) trackStream;
}

int Track::getActive()
{
    return BASS_ChannelIsActive(trackStream);
}

TrackInfo* Track::getInfo(TrackInfo *bufferInfo)
{
    if (trackStream)
    {
        BASS_ChannelGetInfo(trackStream, &bufferInfo->bassInfo);
        bufferInfo->size = BASS_ChannelGetLength(trackStream, BASS_POS_BYTE);
        bufferInfo->time = BASS_ChannelBytes2Seconds(trackStream, bufferInfo->size);
        int lenFile = BASS_StreamGetFilePosition(trackStream, BASS_FILEPOS_END);
        DWORD bitrate=(DWORD)(lenFile/(125*bufferInfo->time)+0.5); // bitrate (Kbps)
        bufferInfo->bitrate = bitrate;
        bufferInfo->trackTags = getTag();
        return bufferInfo;
    }
    return 0;
}

TrackInfo *Track::getTrackInfo()
{
    return &trackInfo;
}

double Track::getPositionTime()
{
    int size = BASS_ChannelGetPosition(trackStream, BASS_POS_BYTE);
    return BASS_ChannelBytes2Seconds(trackStream, size);
}

int Track::isPlaying()
{
    return BASS_ChannelIsActive(trackStream);
}

void Track::signalEndEx()
{
    emit trackEnd();
    qDebug() << "Track end";
}

QStringList Track::getTag()
{
    const char *tag;
    const char* const fmt1 = "%IFV2(%TITL, %ICAP(%TITL), ?)|%IFV2(%ARTI, %ICAP(%ARTI), ?)|%IFV2(%ALBM, %ICAP(%ALBM), ?)";
    tag = TAGS_Read(trackStream, fmt1);
    QString tagsString = QString(tag);
    QStringList tags = tagsString.split("|");
    return tags;
}

QString Track::getName()
{
    return trackName;
}

void Track::setVolume(double volume)
{
    if (trackStream)
    {
        BASS_ChannelSetAttribute(trackStream, BASS_ATTRIB_VOL, volume);
    }
    this->volume = volume;
}

void Track::setPositionTime(double time)
{
    QWORD pos = BASS_ChannelSeconds2Bytes(trackStream, time);
    BASS_ChannelSetPosition(trackStream, pos, BASS_POS_BYTE);
}

void Track::setPositionByte(QWORD byte)
{
    BASS_ChannelSetPosition(trackStream, byte, BASS_POS_BYTE);
}

int Track::GetPositionByte()
{
    return BASS_ChannelGetPosition(trackStream, BASS_POS_BYTE);
}

bool Track::isTrackEnd()
{
    return endPlay;
}

void Track::deleteStream()
{
    BASS_StreamFree(trackStream);
    trackStream = 0;
}
void CALLBACK callbackEnd(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    Track* track = (Track*) user;
    track->signalEndEx();
}
