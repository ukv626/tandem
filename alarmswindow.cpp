#include <QtGui>
#include <QTcpServer>
#include <QTcpSocket>
#include <algorithm>

#include "alarmswindow.h"
#include "ImagesDialog.h"

// -- AlarmsWindow -----------------------------------------------
AlarmsWindow::AlarmsWindow(QWidget *parent)
  : QWidget(parent), nextBlockSize(0), valid_(true)
{
  // label_ = new QLabel(trUtf8("<b>OK</b>"));
  // label_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  // lineEdit = new QLineEdit;
  // label->setBuddy(lineEdit);

  QDir dir("./images");
  QStringList filters;
  filters << "*.eml";

  eventsTable_ = new QTableWidget(0, 4);
  eventsTable_->setHorizontalHeaderLabels(QStringList() << trUtf8("Дата")
	 << trUtf8("Событие") << trUtf8("Пост") << trUtf8("Шлейф"));
  
  messagesTable_ = new QTableWidget(0, 2);
  messagesTable_->setHorizontalHeaderLabels(QStringList() << trUtf8("Дата")
					  << trUtf8("Сообщение"));
  
  foreach(QString file, dir.entryList(filters, QDir::Dirs, QDir::Time)) {
    int row = messagesTable_->rowCount();
    messagesTable_->insertRow(row);
    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    messagesTable_->setItem(row, 0, item0);
    messagesTable_->item(row,0)->setText(QFileInfo(dir,file).created().toString("yyyy-MM-dd hh:mm"));

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    messagesTable_->setItem(row, 1, item1);
    messagesTable_->item(row, 1)->setText(file);
  }
  messagesTable_->resizeColumnsToContents();
  messagesTable_->setAlternatingRowColors(true);
  messagesTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
  messagesTable_->setFixedWidth(messagesTable_->horizontalHeader()->length()+42);

  QAction *showMessageAction = new QAction(trUtf8("Просмотр.."), this);
  //showMessageAction->setShortcut(tr("Ins"));
  connect(showMessageAction, SIGNAL(triggered()), this, SLOT(showMessage()));

  messagesTable_->addAction(showMessageAction);
  messagesTable_->setContextMenuPolicy(Qt::ActionsContextMenu);

  QHBoxLayout *topLayout = new QHBoxLayout;
  // topLayout->addWidget(label_);
  topLayout->addWidget(eventsTable_);
  // topLayout->addStretch();
  topLayout->addWidget(messagesTable_);
  
    
  // QVBoxLayout *mainLayout = new QVBoxLayout; 
  // mainLayout->addLayout(topLayout);
  // mainLayout->addWidget(tableView);
  setLayout(topLayout);
 
  setWindowTitle(tr("Alarms"));

  tcpServer_ = new QTcpServer(this);

  if (!tcpServer_->listen(QHostAddress::Any, 45000)) {
    QMessageBox::critical(0, trUtf8("Ошибка"), tcpServer_->errorString());
    valid_ = false;
  }
  else
    connect(tcpServer_, SIGNAL(newConnection()), this, SLOT(accept()));

  readEvents("./events.txt");
}

void AlarmsWindow::newEvent(QByteArray &data)
{
  int sharpInd   = data.indexOf("#");
  int leftBrInd  = data.indexOf("[");
  int rightBrInd = data.indexOf("]");
  int pipeInd    = data.indexOf("|");

  QString msgType     = data_.mid(8, 7);    
  QString msgSeq     = data.mid(15 + 1, sharpInd -15 - 1);
  QString msgAct     = data.mid(sharpInd + 1, leftBrInd - sharpInd - 1);
  QString msgOpt     = data.mid(leftBrInd + 1, rightBrInd - leftBrInd - 1);
  QString msgOptTail = data.mid(pipeInd + 1, rightBrInd - pipeInd - 1);

  data_.remove(leftBrInd + 1, rightBrInd - leftBrInd - 1);
  data_.replace(msgType, QByteArray("ACK"));
    
  data_.replace(3, 4, QString("%1").arg(data.size()-8, 4, 16, QChar('0'))
		.toUpper().toAscii());
  // label_->setText(label_->text() + "<br>" +
  // 		  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")+ " " +
  // 		  "READ: " +
  // 		  "<font color=blue> <b>\"" + msgType + "\"</b></font>" +
  // 		  "<i>"+ msgSeq + "</i>#" +
  // 		  "<b>"+ msgAct + "</b>" +
  // 		  "<i>["+ msgOpt + "]</i>");
  //qDebug() << data_.mid(0,3).toHex().toUpper() << data_.right(1).toHex().toUpper();

  
  std::vector<Event>::iterator i = find_if(events_.begin(), events_.end(),
					   compareEventId(msgOptTail.mid(1, 3).toInt()));

  QString event_text(trUtf8("Неизвестное событие.."));
	
  if(i != events_.end())
    event_text = (*i).text;
  // label_->setText(label_->text() + "<br>" + event_text);
      

  int row = 0;//messagesTable_->rowCount();
  eventsTable_->insertRow(row);
  
  QTableWidgetItem *item0 = new QTableWidgetItem;
  item0->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
  eventsTable_->setItem(row, 0, item0);
  eventsTable_->item(row,0)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));

  QTableWidgetItem *item1 = new QTableWidgetItem;
  item1->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
  eventsTable_->setItem(row, 1, item1);
  eventsTable_->item(row, 1)->setText(event_text);

  QTableWidgetItem *item2 = new QTableWidgetItem;
  item2->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
  eventsTable_->setItem(row, 2, item2);
  eventsTable_->item(row, 2)->setText(msgType +  msgSeq + "#" + msgAct +
				      "["+ msgOpt + "]");

  if(msgType == "NEW-MSG")
	emit newMailMsg(msgAct);
}

void AlarmsWindow::newMessage(const QString &file)
{
  QDir dir("./images");
  int row = 0;//messagesTable_->rowCount();
  messagesTable_->insertRow(row);
  
  QTableWidgetItem *item0 = new QTableWidgetItem;
  item0->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
  messagesTable_->setItem(row, 0, item0);
  messagesTable_->item(row,0)->setText(QFileInfo(dir,file).created().toString("yyyy-MM-dd hh:mm"));

  QTableWidgetItem *item1 = new QTableWidgetItem;
  item1->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
  messagesTable_->setItem(row, 1, item1);
  messagesTable_->item(row, 1)->setText(file);
}


void AlarmsWindow::accept()
{
  qDebug() << "accept()";
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
  qDebug() << "readMsg()";
  QTcpSocket* clientConnection = (QTcpSocket *)sender();

  QByteArray dataTmp;
  for(;;) {
    if(nextBlockSize == 0) {
      if(clientConnection->bytesAvailable() < HeaderLength) {
	// qDebug() << "break 1: " << clientConnection->bytesAvailable();
	break;
      }

      dataTmp = clientConnection->read(HeaderLength);
      if(dataTmp.at(0) == 0x0A) {
	char *offset;
	nextBlockSize=strtol(dataTmp.right(sizeof(quint16)), &offset, 16);
	data_.append(dataTmp);
      }
    }

    if(clientConnection->bytesAvailable() < nextBlockSize) {
      // qDebug() << "break 2" << nextBlockSize << clientConnection->bytesAvailable();
      break;
    }
    
    dataTmp = clientConnection->read(nextBlockSize+1);
    data_.append(dataTmp);
    QString msgType = data_.mid(8, 7);

    if(msgType=="ADM-CID" || msgType=="NEW-MSG") {
      //newEvent
      newEvent(data_);
      
      clientConnection->write(data_);
    }
    else qDebug() << data_;
    nextBlockSize=0;
    data_.truncate(0);
  }
}

void AlarmsWindow::showMessage()
{
  QString str = messagesTable_->item(messagesTable_->currentRow(), 1)->text();

  ImagesDialog *dlg = new ImagesDialog(str, this);
  dlg->setAttribute(Qt::WA_DeleteOnClose);
  dlg->show();
}


void AlarmsWindow::readEvents(const QString &filename)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::warning(0, trUtf8("Ошибка"),
			 trUtf8("Не найден файл events.txt !!"));
    return;
  }

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList fields = line.split(';');
    if(fields.size() == 4) {
      qint16 id    = fields.takeFirst().toInt();
      qint8 type   = fields.takeFirst().toInt();
      bool alert   = fields.takeFirst().toInt();
      QString text = fields.takeFirst();
      Event event  = { id, type, alert, text };
      events_.push_back(event);
    }
  }

  if(events_.size() == 0) {
    QMessageBox::warning(0, trUtf8("Ошибка"), trUtf8("Ничего не загружено из файла events.txt !!"));
    return;
  }

  std::sort(events_.begin(), events_.end());
}
