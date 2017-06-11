#ifndef FLIPPING_H
#define FLIPPING_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

class flipping : public QWidget
{
    Q_OBJECT
public:
   explicit flipping(QWidget *parent = 0);
    ~flipping();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // FLIPPING_H
