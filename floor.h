#ifndef FLOOR_H
#define FLOOR_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

class FLOOR : public QWidget
{
    Q_OBJECT
public:
   explicit FLOOR(QWidget *parent = 0);
    ~FLOOR();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // FLOOR_H
