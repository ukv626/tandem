#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>
#include <QVector>
#include <QFile>

class FlexWatchCam;

class CameraDialog : public QDialog
{
    Q_OBJECT

public:
  CameraDialog(const QString &ip,
	       int postNum,
	       int camNum1, int camNum2,
	       QWidget *parent = 0);
    
private:
  FlexWatchCam *flexWatchCam1_;
  FlexWatchCam *flexWatchCam2_;
  /* QFile file_; */
};

#endif
