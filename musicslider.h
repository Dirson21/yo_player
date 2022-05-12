#ifndef MUSICSLIDER_H
#define MUSICSLIDER_H
#include <QObject>
#include <QWidget>
#include <QSlider>
#include <QMouseEvent>
#include <QDebug>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QRect>
class MusicSlider : public QSlider
{
    Q_OBJECT
public:
    MusicSlider(QWidget* parent = nullptr);
private:
    int pixelPosToRangeValue(QPoint pos);
protected:
    void mousePressEvent ( QMouseEvent * event )
    {
        if (event->button() == Qt::LeftButton)
        {
            int val = pixelPosToRangeValue(event->pos());
            setValue(val);
            QSlider::mousePressEvent(event);
        }
    }
    void mouseReleaseEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
        {
            //emit sliderReleased();
        }
        QSlider::mouseReleaseEvent(event);
    }

};




#endif // MUSICSLIDER_H
