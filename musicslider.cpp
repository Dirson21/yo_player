#include "musicslider.h"

MusicSlider::MusicSlider(QWidget* parent)
{
    QSlider((QWidget*)parent);

}
int MusicSlider::pixelPosToRangeValue(QPoint pos)
{
    int sliderLenght;
    int sliderMin;
    int sliderMax;
    QStyleOptionSlider *opt = new QStyleOptionSlider;
    this->initStyleOption(opt);
    QRect gr = this->style()->subControlRect(QStyle::CC_Slider, opt,QStyle::SC_SliderGroove, this);
    QRect sr = this->style()->subControlRect(QStyle::CC_Slider, opt,QStyle::SC_SliderHandle, this);
    if (orientation() == Qt::Horizontal)
    {
        sliderLenght = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLenght + 1;
    }
    else
    {
        sliderLenght = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLenght + 1;
    }

    QPoint pr = pos - sr.center() + sr.topLeft();
    int p;
    if (orientation() == Qt::Horizontal)
        p = pr.x();
    else p = pr.y();
    return QStyle::sliderValueFromPosition(minimum(), maximum(), p - sliderMin,
                                           sliderMax - sliderMin, opt->upsideDown);
}
