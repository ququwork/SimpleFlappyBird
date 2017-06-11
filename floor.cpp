#include "floor.h"

FLOOR::FLOOR(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(405,40);
}

FLOOR::~FLOOR()
{

}

void FLOOR::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":Image/floor.png");
    painter.drawPixmap(0,0,405,40,pix);
}
