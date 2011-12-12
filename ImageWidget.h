#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

#include "ImageWidget.h"

class QPixmap;

class ImageWidget : public QWidget
{
  Q_OBJECT

public:
  ImageWidget(QWidget *parent = 0);
  QSize sizeHint() const;
  void draw(QPainter *painter);
  void setImage(const QPixmap &img);

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPixmap image_;
};

#endif
