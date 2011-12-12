#include <QtGui>
#include <QTcpServer>
#include <QTcpSocket>

#include "alarmswindow.h"

// -- AlarmsWindow -----------------------------------------------
AlarmsWindow::AlarmsWindow(QWidget *parent)
  : QWidget(parent)
{
  label_ = new QLabel(trUtf8("<b>проверка связи</b>"));
  // lineEdit = new QLineEdit;
  // label->setBuddy(lineEdit);

  QVBoxLayout *topLayout = new QVBoxLayout;
  topLayout->addWidget(label_);
  topLayout->addStretch();
    
  // QVBoxLayout *mainLayout = new QVBoxLayout; 
  // mainLayout->addLayout(topLayout);
  // mainLayout->addWidget(tableView);
  setLayout(topLayout);
 
  setWindowTitle(tr("Alarms"));

  tcpServer_ = new QTcpServer(this);

  if (!tcpServer_->listen(QHostAddress::Any, 45000)) {
    label_->setText(trUtf8("<b>Unable to start the server:</b> %1.").arg(tcpServer_->errorString()));
  }
  else
    connect(tcpServer_, SIGNAL(newConnection()), this, SLOT(accept()));
}

void AlarmsWindow::accept()
{
  // QByteArray block;
  // QDataStream out(&block, QIODevice::WriteOnly);
  // out.setVersion(QDataStream::Qt_4_0);
  // out << (quint16)0;
  // out << fortunes.at(qrand() % fortunes.size());
  // out.device()->seek(0);
  // out << (quint16)(block.size() - sizeof(quint16));

  QTcpSocket *clientConnection = tcpServer_->nextPendingConnection();

  //int clientSockId = clientConnection->socketDescriptor();
  //clients_[clientSockId] = clientConnection;

  connect(clientConnection, SIGNAL(readyRead()),
	  this, SLOT(readMsg()));

  connect(clientConnection, SIGNAL(disconnected()),
	  clientConnection, SLOT(deleteLater()));
}

void AlarmsWindow::readMsg()
{
  QTcpSocket* clientConnection = (QTcpSocket *)sender();

  QByteArray dataTmp;
  for(;;) {
    if(nextBlockSize == 0) {
      if(clientConnection->bytesAvailable() < HeaderLength)
	break;

      dataTmp = clientConnection->read(HeaderLength);
      if(dataTmp.at(0) == 0x0A) {
	char *offset;
	nextBlockSize=strtol(dataTmp.right(sizeof(quint16)), &offset, 16);
	data_.append(dataTmp);
      }
    }

    if(clientConnection->bytesAvailable() < nextBlockSize)
      break;
    
    dataTmp = clientConnection->read(nextBlockSize+1);
    data_.append(dataTmp);
    QString msgType = data_.mid(8, 7);

    if(msgType=="ADM-CID" || msgType=="NEW-MSG") {
      int sharpInd=data_.indexOf("#");
      int leftBrInd=data_.indexOf("[");
      int rightBrInd=data_.indexOf("]");
      
      
      //QString msgSeq = data_.mid(16, 6);
      QString msgSeq = data_.mid(15 + 1, sharpInd -15 - 1);
      QString msgAct = data_.mid(sharpInd + 1, leftBrInd - sharpInd - 1);
      QString msgOpt = data_.mid(leftBrInd + 1, rightBrInd - leftBrInd - 1);
      
      label_->setText(label_->text() + "<br>" +
		      QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")+ " " +
		      "READ: " +
		      "<font color=blue> <b>\"" + msgType + "\"</b></font>" +
		      "<i>"+ msgSeq + "</i>#" +
		      "<b>"+ msgAct + "</b>" +
		      "<i>["+ msgOpt + "]</i>");
      //qDebug() << data_.mid(0,3).toHex().toUpper() << data_.right(1).toHex().toUpper();

      data_.remove(leftBrInd + 1, rightBrInd - leftBrInd - 1);
      data_.replace(msgType, QByteArray("ACK"));
    
      data_.replace(3, 4, QString("%1").arg(data_.size()-8, 4, 16, QChar('0'))
		    .toUpper().toAscii());
      //data_.replace(2, 4, QString::number(data_.size()-4, 16).toUpper().toAscii());

      label_->setText(label_->text() + "<br>WRITE: " +
		      data_.mid(7, data_.size()-8));
      //qDebug() << data_.mid(0,3).toHex().toUpper() << data_.right(1).toHex().toUpper();
      clientConnection->write(data_);

      if(msgType=="NEW-MSG")
	emit newMailMsg();
    }
    else qDebug() << data_;
    nextBlockSize=0;
    data_.truncate(0);
  }
}
