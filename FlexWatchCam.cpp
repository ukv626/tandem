#include <QtGui>

#include "FlexWatchCam.h"
#include "ImageWidget.h"


FlexWatchCam::FlexWatchCam(const QString &ip, int camNum, QWidget *parent)
  : ImageWidget(parent)
{
  connect(&http_, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
  connect(&http_, SIGNAL(readyRead(const QHttpResponseHeader &)),
	  this, SLOT(httpReadyRead(const QHttpResponseHeader &)));
  
  boundary_.append(QString("--myboundary"));
  QSettings settings("./tandem.conf", QSettings::IniFormat);
  int fps = settings.value("fps", "30").toInt();

  http_.setHost(ip);
  http_.get(QString("/cgi-bin/fwstream.cgi?FwModId=0&AppKey=0x0450f000&PortId=%1&PauseTime=%2&FwCgiVer=0x0001\r\n\r\n").arg(camNum-1).arg(100 / fps));
  http_.close();
}

void FlexWatchCam::httpDone(bool error)
{
  if(error && http_.error() != QHttp::Aborted)
    qDebug() << "Error: " << qPrintable(http_.errorString());
}


void FlexWatchCam::processing(const QByteArray &array)
{
  static const char new_lines_data[] = { 0x0D, 0x0A, 0x2D, 0x2D, 0x0D, 0x0A };
  static const QByteArray new_lines = QByteArray::fromRawData(new_lines_data,
  							      sizeof(new_lines_data));

  int boundary_pos = array.indexOf(boundary_);
  if(boundary_pos == -1) {
    image_.append(array);
  }
  else {
    // если перед --myboundary идет завершение пред. картинки - добавляем ее  
    if(boundary_pos > 0) {
      image_.append(array.left(boundary_pos));
    }

    if(image_.size() > 0) {
      QPixmap pixmap;
      pixmap.loadFromData(image_);
      setImage(pixmap);
      
      image_.truncate(0);
    }

    // если в этом же пакете после "--myboundary" идет начало след. фотки
    if(array.size() > boundary_pos + 58 + 4) {
      processing(array.mid(boundary_pos + 58 + 4));
    }
  }
}


void FlexWatchCam::httpReadyRead(const QHttpResponseHeader & resp)
{
  if(resp.statusCode() != 200)
    QMessageBox::warning(0, "Error", tr("StatusCode = %1").arg(resp.statusCode()));
  else {
    QByteArray array = http_.readAll();
    processing(array);
  }
}
