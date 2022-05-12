#include "playlist.h"
#include <QDebug>

PlayList::PlayList()
{

    currentTrack = new Track();
    connect(this, &PlayList::playlistEnd, [this]()
    {
        if (currentPos >-1)
        {
            currentPos = tracksName.size() - 1;
        }
        else
        {
            currentPos = 0;
        }
        if (tracksName.size())
        {
            currentTrack->setTrack(tracksName[currentPos]);
        }
        else
        {
            currentPos = -1;
        }
    });

    connect(currentTrack, SIGNAL(trackEnd()), SLOT(next()));
}

PlayList::~PlayList()
{
    delete currentTrack;
}

void PlayList::addTrack(QString TrackName)
{
    tracksName.push_back(TrackName);
}

void PlayList::play()
{
    if (tracksName.size())
    {
        if (!currentTrack->isStream())
            next();
        TrackInfo *trackInfo = currentTrack->getTrackInfo();
        currentTrack->play();
        ispause = 0;
        return;
    }
}

void PlayList::changeTrack(int pos)
{
    if (!tracksName.size()) return;
    int oldPos = currentPos;
    currentPos = pos;
    if (currentPos < tracksName.size() && currentPos > -1)
    {
        TrackInfo *trackInfo = currentTrack->getTrackInfo();
        double size = currentTrack->getPositionTime();
        bool isEnd = (int)trackInfo->time == (int)size && trackInfo->size > 0;
        int isplay = currentTrack->isPlaying();
        currentTrack->setTrack(tracksName[currentPos]);
        if (isplay == BASS_ACTIVE_PLAYING || isEnd > 0)
        {
            play();
        }
        emit trackPosChange(oldPos,currentPos);
        qDebug () << "Track change";
        qDebug () << "Old pos "<< oldPos;
        qDebug() << "New pos" << currentPos;
        return;
    }
    emit playlistEnd();
}

int PlayList::nextPos(int pos)
{
    if (tracksName.size())
    {
        if (repeat) pos = (pos + 1) % tracksName.size();
        else pos++;

        if (pos >= tracksName.size()) pos = tracksName.size();
        return pos;
    }
    return -1;
}

int PlayList::prevPos(int pos)
{
    pos--;
    if (pos < 0)
    {
        if (repeat) pos = tracksName.size()-1;
    }
    return pos;
}

void PlayList::next()
{
    int pos = nextPos(currentPos);
    changeTrack(pos);
}

void PlayList::prev()
{
    int pos = prevPos(currentPos);
    changeTrack(pos);
}

int PlayList::getCurrentPos()
{
    return currentPos;
}

Track* PlayList::getCurrentTrack()
{
    return currentTrack;
}

void PlayList::setRepeat(bool repeat)
{
    this->repeat = repeat;
}

void PlayList::deleteTrack(int pos)
{
    if (tracksName.size() && pos < tracksName.size())
    {
        if (pos <=0)
        {
            pos = 0;
            tracksName.erase(tracksName.begin());
        }
        else
        {
            tracksName.erase(tracksName.begin() + pos);
        }

        if (pos == currentPos)
        {
            currentTrack->deleteStream();
            changeTrack(currentPos);

        }
        emit trackDeleted();
        if (!tracksName.size())
        {
            emit playlistEnd();
            emit tracksNameEnd();
        }
        if (currentPos > -1)
            emit trackPosChange(currentPos, currentPos);
    }
}

void PlayList::pause()
{
    currentTrack->pause();
    ispause = 1;
}


int PlayList::getCountTracks()
{
    return tracksName.size();
}

void PlayList::stop()
{
    currentTrack->stop();
}
