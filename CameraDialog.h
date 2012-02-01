#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>
#include <QHttp>
#include <QUrl>
#include <QByteArray>

class QLabel;
class QComboBox;
class QSpinBox;
class ImageWidget;
class QDialogButtonBox;

class CameraDialog : public QDialog
{
    Q_OBJECT

public:
    CameraDialog(QWidget *parent = 0);

/* signals: */
/*   void done(); */

private slots:
  bool getStream();
  void httpDone(bool error);
  void httpReadyRead(const QHttpResponseHeader & resp);
  void start();
  void stop();

private:
    QHttp http_;
    QByteArray boundary_;
    QByteArray image_;

    QLabel *ipLabel_;
    QLabel *cameraLabel_;
    QLabel *fpsLabel_;
    
    QComboBox *ipComboBox_;
    QComboBox *cameraComboBox_;
    QSpinBox *fpsSpinBox_;
    
    ImageWidget *imageWidget_;
    bool stopFlag_;

    /* QPushButton *startButton; */
    /* QPushButton *stopButton; */
    /* QPushButton *closeButton; */
    
    QDialogButtonBox *buttonBox_;
};

#endif
