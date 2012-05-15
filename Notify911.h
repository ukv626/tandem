#ifndef NOTIFY911_H
#define NOTIFY911_H

#include <QTcpSocket>
#include <QByteArray>

class QTimer;

class Notify911 : public QObject
{
    Q_OBJECT

public:
  Notify911(const QString &ip, quint16 port, const QByteArray &data,
	    QObject *parent = 0);
  ~Notify911();

private slots:
  void sendAlarm();
  void readyReadAck();
  /* void connectionClosedByServer(); */
  void error();

private:
  void closeConnection();
  
  QTcpSocket tcpSocket_;
  QByteArray data_;

  QTimer *timer_;
};

#endif
