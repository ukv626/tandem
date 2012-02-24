#ifndef FLEXWATCHCAM_H
#define FLEXWATCHCAM_H

#include <QHttp>
#include <QUrl>
#include <QByteArray>
#include "ImageWidget.h"

class FlexWatchCam : public ImageWidget
{
    Q_OBJECT

public:
  FlexWatchCam(const QString &ip, int camNo, QWidget *parent = 0);

/* signals: */
/*   void done(); */

private slots:
  void httpDone(bool error);
  void httpReadyRead(const QHttpResponseHeader & resp);

private:
  void processing(const QByteArray &array);
  QHttp http_;
  QByteArray boundary_;
  QByteArray image_;
};

#endif
