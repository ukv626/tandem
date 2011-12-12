#include <QWidget>
#include <QPixmap>
#include <QPainter>

#include "ImageWidget.h"

ImageWidget::ImageWidget(QWidget *parent)
  : QWidget(parent)
{
}

void ImageWidget::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  draw(&painter);
}

QSize ImageWidget::sizeHint() const
{
  return QSize(352, 288);
}

void ImageWidget::draw(QPainter *painter)
{
  painter->drawPixmap(QPoint(0,0), image_);
}

void ImageWidget::setImage(const QPixmap &img)
{
  image_ = img;
  repaint();
}

