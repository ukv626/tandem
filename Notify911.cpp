#include <QDebug>
#include <QTimer>

#include "Notify911.h"

Notify911::Notify911(const QString &ip, quint16 port,const QByteArray &data,
		     QObject *parent)
: QObject(parent), data_(data)
{
  qDebug() << __PRETTY_FUNCTION__;

  timer_ = new QTimer(this);
  connect(timer_, SIGNAL(timeout()), this, SLOT(error()));
  
  connect(&tcpSocket_, SIGNAL(connected()), this, SLOT(sendAlarm()));
  // connect(&tcpSocket_, SIGNAL(disconnected()),
  // 	  this, SLOT(connectionClosedByServer()));
  connect(&tcpSocket_, SIGNAL(readyRead()),
	  this, SLOT(readyReadAck()));
  connect(&tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
	  this, SLOT(error()));
  
  tcpSocket_.connectToHost(ip, port);
}

Notify911::~Notify911()
{
  qDebug() << __PRETTY_FUNCTION__;
}

void Notify911::sendAlarm()
{
  tcpSocket_.write(data_);

  timer_->start(3000);
}


void Notify911::readyReadAck()
{
  // Получили ответ

  // Закрываем соединение
  closeConnection();
}


void Notify911::closeConnection()
{
  if(timer_->isActive())
    timer_->stop();

  tcpSocket_.close();
  deleteLater();
}

// void Notify911::connectionClosedByServer()
// {
//   qDebug() << __PRETTY_FUNCTION__;
//   closeConnection();
// }

void Notify911::error()
{
  if(timer_->isActive())
    timer_->stop();

  // Закрываем соединение
  closeConnection();
}
