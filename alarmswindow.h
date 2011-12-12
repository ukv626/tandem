#ifndef ALARMSWINDOW_H
#define ALARMSWINDOW_H

#include <QWidget>
#include <QByteArray>

class QLabel;
class QLineEdit;
class QTcpServer;



// -- AlarmsWindow -----------------------------------------------
class AlarmsWindow : public QWidget
{
    Q_OBJECT

public:
  enum { HeaderLength = 7 };
  AlarmsWindow(QWidget *parent = 0);

private slots:
  void accept();
  void readMsg();

signals:
  void newMailMsg();

private:
  QLabel *label_;
    
  QTcpServer *tcpServer_;
  QByteArray data_;
  quint8 nextBlockSize;
    /* QLineEdit *lineEdit; */
};

#endif
