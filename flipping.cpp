#include "flipping.h"

flipping::flipping(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(35,35);
}

flipping::~flipping()
{
}

void flipping::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":Image/bird3.png");
    painter.drawPixmap(0,0,35,35,pix);
}
