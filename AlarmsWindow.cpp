#include <QtGui>
#include <QtSql>
#include <QTcpServer>
#include <QTcpSocket>
#include <algorithm>

#include "AlarmsWindow.h"
#include "ImagesDialog.h"
#include "Notify911.h"

AlarmsTableView::AlarmsTableView(QWidget *parent)
  : QTableView(parent)
{
}

void AlarmsTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
  emit(doubleClicked(this->indexAt(QPoint(event->pos().x(), event->pos().y()))));
  event->accept();
}


AlarmsQueryModel::AlarmsQueryModel(QObject *parent)
  : QSqlQueryModel(parent)
{
  refresh();
}

QVariant AlarmsQueryModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlQueryModel::data(index, role);
  switch (role) {
  case Qt::FontRole:  //BackgroundColorRole
    if (index.sibling(index.row(), IsRead).data(Qt::DisplayRole).toInt() == 0) {
      return qVariantFromValue(QFont("default", 10, QFont::Bold));
      //qVariantFromValue(QColor(Qt::gray));
    }
    else
      return value;
    
  case Qt::TextColorRole:  //BackgroundColorRole
    return qVariantFromValue(QColor(index.sibling(index.row(), Type).data(Qt::DisplayRole).toString()));

  case Qt::DisplayRole:
    // if (index.column() == Date)
    //   return value.toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    // if (index.column() == IsMessage) {
    //   if(!value.toBool())
    //     return trUtf8("");
    //   else 
    //     return trUtf8("Фото");
    // }
    if (index.column() == Q) {
      if(value.toInt() == 3)
        return trUtf8("ВОССТАНОВЛ.");
      else 
        return trUtf8("");
    }
    else
      return value;

    
      case Qt::TextAlignmentRole: // Выравнивание
        if(index.column() == Gg || index.column() == Zzz )
	  return double(Qt::AlignRight | Qt::AlignVCenter);
        else
	  return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}

bool AlarmsQueryModel::setData(const QModelIndex &index,
		      const QVariant &value,
		      int /* role */)
{
  if (index.column() != IsRead)
    return false;

  QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
  int id = QSqlQueryModel::data(primaryKeyIndex).toInt();
  bool ok;
  QSqlQuery query;
  if (index.column() == IsRead) {
    query.prepare("update tb_logs set isRead = ? where id = ?");
    query.addBindValue(value.toInt());
    query.addBindValue(id);
  }
  ok = query.exec();
  refresh();
  return ok;
}

void AlarmsQueryModel::refresh()
{
  setQuery(" SELECT l.id, l.date_, l.act, l.q, e.text, l.gg, l.zzz"
	   " ,l.isRead,et.text,e.isAlert"
	   " FROM tb_logs l"
	   " ,tb_events e" 
	   " ,tb_eventsType et"
	   " WHERE l.eee=e.id"
	   " AND e.type=et.id"
	   " AND e.isVisible=1"
	   " ORDER BY l.date_ DESC");

  setHeaderData(Date, Qt::Horizontal, trUtf8("Дата"));
  setHeaderData(Act, Qt::Horizontal, trUtf8("Объект"));
  setHeaderData(Q, Qt::Horizontal, trUtf8("Q"));
  setHeaderData(Eee, Qt::Horizontal, trUtf8("Сообщение"));
  setHeaderData(Gg, Qt::Horizontal, trUtf8("Пост"));
  setHeaderData(Zzz, Qt::Horizontal, trUtf8("Шлейф"));
}


// -- AlarmsWindow -----------------------------------------------
AlarmsWindow::AlarmsWindow(QWidget *parent)
  : QWidget(parent), nextBlockSize(0), valid_(false)
{
  QLabel *label = new QLabel(trUtf8("<H2>Пост 1</H2>"));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label->setStyleSheet("background-color: lightGrey; border: 2px solid grey");
  labels.append(label);
  
  label = new QLabel(trUtf8("<H2>Пост 2</H2>"));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label->setStyleSheet("background-color: lightGrey; border: 2px solid grey");
  labels.append(label);
  
  label = new QLabel(trUtf8("<H2>Пост 3</H2>"));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label->setStyleSheet("background-color: lightGrey; border: 2px solid grey");
  labels.append(label);

  label = new QLabel(trUtf8("<H2>Пост 4</H2>"));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label->setStyleSheet("background-color: lightGrey; border: 2px solid grey");
  labels.append(label);

  label = new QLabel(trUtf8("<H2>ВШ 530</H2>"));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label->setStyleSheet("background-color: lightGrey; border: 2px solid grey");
  labels.append(label);

  textBrowser_ = new QTextBrowser();
  textBrowser_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  textBrowser_->setFixedWidth(420);

  tableView_ = new AlarmsTableView(this);
  tableModel_ = new AlarmsQueryModel(this);
  
  // tableModel_->setTable("tb_logs");
  // tableModel_->setRelation(AlarmsRelTableModel::Eee,
  // 			   QSqlRelation("tb_events", "id", "text"));
  // tableModel_->setSort(AlarmsRelTableModel::Date, Qt::DescendingOrder);
  // //tableModel_->setEditStrategy(QSqlTableModel::OnManualSubmit);
  // tableModel_->select();

  tableView_->setModel(tableModel_);
  tableView_->setItemDelegate(new QSqlRelationalDelegate(tableView_));

  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  tableView_->setColumnHidden(AlarmsQueryModel::Id, true);
  tableView_->setColumnHidden(AlarmsQueryModel::Act, true);
  tableView_->setColumnHidden(AlarmsQueryModel::Zzz, true);
  tableView_->setColumnHidden(AlarmsQueryModel::Q, true);
  tableView_->setColumnHidden(AlarmsQueryModel::IsRead, true);
  tableView_->setColumnHidden(AlarmsQueryModel::Type, true);
  tableView_->setColumnHidden(AlarmsQueryModel::IsAlert, true);
  
  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  tableView_->setAlternatingRowColors(true);

  tableView_->setColumnWidth(AlarmsQueryModel::Eee, 335);
  
  QAction *showMessageAction = new QAction(trUtf8("Просмотр.."), this);
  //showMessageAction->setShortcut(tr("Ins"));
  connect(showMessageAction, SIGNAL(triggered()), this, SLOT(showMessage()));

  tableView_->addAction(showMessageAction);
  tableView_->setContextMenuPolicy(Qt::ActionsContextMenu);

  connect(tableView_, SIGNAL(doubleClicked(QModelIndex)),
  	  this, SLOT(doubleClick(QModelIndex)));

  connect(tableView_->selectionModel(),
  	  SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
  	  SLOT(showPictures(const QModelIndex &, const QModelIndex &)));
  
  tableView_->selectRow(0);
  
  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(labels.at(0));
  topLayout->addWidget(labels.at(1));
  topLayout->addWidget(labels.at(2));
  topLayout->addWidget(labels.at(3));
  topLayout->addWidget(labels.at(4));

  QHBoxLayout *centerLayout = new QHBoxLayout;
  centerLayout->addWidget(tableView_);
  centerLayout->addWidget(textBrowser_);
  
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(topLayout);
  layout->addLayout(centerLayout);
  // topLayout->addWidget(label_);
  // topLayout->addStretch();
  // topLayout->addWidget(messagesTable_);
    
  // QVBoxLayout *mainLayout = new QVBoxLayout; 
  // mainLayout->addLayout(topLayout);
  // mainLayout->addWidget(tableView);
  setLayout(layout);
 
  setWindowTitle(tr("Alarms"));
  // setFixedWidth(tableView_->horizontalHeader()->length()+50);

// if(true) {
if(checkLicense()) {
    tcpServer_ = new QTcpServer(this);
    QSettings settings("./tandem.conf", QSettings::IniFormat);
    int port = settings.value("port", "45000").toInt();

    if (!tcpServer_->listen(QHostAddress::Any, port))
      QMessageBox::critical(0, trUtf8("Ошибка"), tcpServer_->errorString());
    else {
      connect(tcpServer_, SIGNAL(newConnection()), this, SLOT(accept()));
      valid_ = true;
    }
  }
}

AlarmsWindow::~AlarmsWindow()
{
  delete tableModel_;
  delete tcpServer_;
}

bool AlarmsWindow::checkLicense()
{
  bool ok2start(false);
  QDir dir("/proc/scsi/usb-storage/");
  QStringList filters("*");

  QByteArray origHashData;
  QFile origFile("./license.txt");
  if (origFile.open(QIODevice::ReadOnly))
    origHashData = origFile.readAll();
  origFile.close();

  foreach(QString filename, dir.entryList(filters, QDir::Files)) {
    QFile file(dir.path() + "/" + filename);
      
    if (file.open(QIODevice::ReadOnly)) {
      QByteArray fileData = file.readLine();
      fileData = file.readAll();

      QByteArray hashData = QCryptographicHash::hash(fileData,
						     QCryptographicHash::Md5);
      if(hashData.toHex() == origHashData)
	ok2start = true;
    }
  }

  if(!ok2start)
    QMessageBox::critical(0, trUtf8("Ошибка"), trUtf8("Неверный ключ!!"));

  return ok2start;
}

void AlarmsWindow::doubleClick(const QModelIndex &index)
{
  int row = index.row();
  QAbstractItemModel *model = tableView_->model();
  QModelIndex ind = model->index(row, AlarmsQueryModel::IsRead);
  model->setData(ind, model->data(ind).toInt() == 0 ? 1 : 0);
  model->submit();
  tableView_->selectRow(row);
}

// QString AlarmsWindow::getColorByType(int type)
// {
//   QString color = "";
//   switch(type) {
//   case 0:
//     color = "green";
//     break;
//   case 1:
//     color = "yellow";
//     break;
//   case 2:
//     color = "red";
//     break;
//   case 3:
//     color = "grey";
//     break;
//   }
//   return color;
// }

bool AlarmsWindow::isAlert(qint16 eee)
{
  bool result(false);
  QSqlQuery query;
  query.prepare("SELECT isAlert FROM tb_events WHERE id=:id");
  query.bindValue(":id", eee);
  if(query.exec() && query.next()) {
    result = query.value(0).toBool();
  }

  qDebug() << eee << result;
  return result;
}

bool AlarmsWindow::newEvent()
{
  // qDebug() << data_; //.mid(0,3).toHex().toUpper()
  
  QString msgType = data_.mid(8, 7);    
  if(msgType != "ADM-CID" && msgType != "NEW-MSG") 
    return false;

  QProcess::startDetached("./beep.sh");
  
  qint8 sharpInd   = data_.indexOf("#");
  qint8 leftBrInd  = data_.indexOf("[");
  qint8 rightBrInd = data_.indexOf("]");
  qint8 pipeInd    = data_.indexOf("|");

  if(sharpInd   == -1 || leftBrInd == -1 ||
     rightBrInd == -1 || pipeInd   == -1)
    return false;
  
  QString msgSeq     = data_.mid(15 + 1, sharpInd -15 - 1);
  QString msgAct     = data_.mid(sharpInd + 1, leftBrInd - sharpInd - 1);
  QString msgOpt     = data_.mid(leftBrInd + 1, rightBrInd - leftBrInd - 1);
  QString msgOptTail = data_.mid(pipeInd + 1, rightBrInd - pipeInd - 1);

  qint8 q    = msgOptTail.left(1).toInt();
  qint16 eee = msgOptTail.mid(1, 3).toInt();
  if(q == 3) eee *= -1;
  qint8 gg   = msgOptTail.mid(5, 2).toInt();


  // Пересылка сообщения на АРМы МЧС
  if(isAlert(eee)) {
      QSettings settings("./tandem.conf", QSettings::IniFormat);
      QString ip = settings.value("ip911_1", "").toString();
      quint32 port = settings.value("port911_1", "45002").toInt();

      if(ip.size() > 0) {
	Notify911 *notifier = new Notify911(ip, port, data_);
      }

      ip = settings.value("ip911_2", "").toString();
      port = settings.value("port911_2", "45002").toInt();

      if(ip.size() > 0) {
	Notify911 *notifier = new Notify911(ip, port, data_);
      }
  }

  
  // Готовим данные (изменяя исходные) для отправки подтверждения
  data_.remove(leftBrInd + 1, rightBrInd - leftBrInd - 1);
  data_.replace(msgType, QByteArray("ACK"));
    
  data_.replace(3, 4, QString("%1").arg(data_.size() - 8, 4, 16,
					QChar('0')).toUpper().toAscii());

  // Добавляем запись в журнал
  QSqlQuery query;
  query.prepare("INSERT INTO tb_logs VALUES(NULL,:Date,:Act,:Q,:Eee,:Gg,:Zzz,0)");
  query.bindValue(":Date",
		  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
  query.bindValue(":Act", msgAct);
  query.bindValue(":Q", q);
  query.bindValue(":Eee", eee);
  query.bindValue(":Gg", gg);
  query.bindValue(":Zzz", msgOptTail.right(3).toInt());
  if(!query.exec())
    QMessageBox::critical(0, trUtf8("Ошибка"), query.lastError().text());
  else {
    // int row = tableView_->selectionModel()->selectedIndexes().first().row();
    int row = tableView_->currentIndex().row();
    tableModel_->refresh();
    tableView_->selectRow(row + 1);

    // Обновляем кнопки индикации
    if(gg > 0 && gg <= labels.size()) {
      QAbstractItemModel *model = tableView_->model();
      QString type = model->data(model->index(0, AlarmsQueryModel::Type)).toString();
    
      labels.at(gg - 1)->setStyleSheet(tr("background-color: %1; "
			"border: 2px solid grey").arg(type));
    }
  } // if(query.exec())
    
  if(msgType == "NEW-MSG")
    emit newMailMsg(msgAct);

  return true;
}

void AlarmsWindow::accept()
{
  // qDebug() << "accept()";
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
  // qDebug() << "readMsg()";
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
	nextBlockSize = strtol(dataTmp.right(sizeof(quint16)), &offset, 16);
	data_.append(dataTmp);
      }
    }

    if(clientConnection->bytesAvailable() < nextBlockSize) {
      // qDebug() << "break 2" << nextBlockSize << clientConnection->bytesAvailable();
      break;
    }
    
    dataTmp = clientConnection->read(nextBlockSize+1);
    data_.append(dataTmp);

    if(newEvent())
      clientConnection->write(data_);
    else
      qDebug() << "!newEvent(); " << data_;
    
    nextBlockSize = 0;
    data_.truncate(0);
  }
}

void AlarmsWindow::showPictures(const QModelIndex &current,
				const QModelIndex & /* previous */)
{
  if(current.row()<0)
    return;
  
  QAbstractItemModel *model = tableView_->model();
  QString str = model->data(model->index(current.row(), 2)).toString();

  if(!str.endsWith(".eml")) {
    textBrowser_->setHtml("");
    return;
  }

  QString page("<html>");
  QDir dir("./images/" + str);

  QFile file(dir.path() + "/mail.info");
  QString msg("<PRE><H3>");
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    QTextStream in(&file);
    while (!in.atEnd()) {
      QString line = in.readLine();
      msg += line + "<BR>";
    }
    msg+="</H3></PRE>";
  }

  page += msg;
  QStringList filters;
  foreach(QByteArray format, QImageReader::supportedImageFormats())
    filters += "*." + format;

  qint8 i = 0;
  foreach(QString file, dir.entryList(filters, QDir::Files)) {
    page += tr("<img src=\"%1\" width=176 height=144 />").arg(file);
    if(i++ & 1)
      page += "<BR>";
  }
  
  page += "</html>";
  
  textBrowser_->setSearchPaths(QStringList() << dir.path());
  textBrowser_->setHtml(page);
}


void AlarmsWindow::showMessage()
{
  QAbstractItemModel *model = tableView_->model();

  QString eee = model->data(model->index(tableView_->currentIndex().row(),
					 AlarmsQueryModel::Eee),
			   Qt::EditRole).toString();

  if(eee == trUtf8("НОВОЕ СООБЩЕНИЕ")) {
    QString str = model->data(model->index(tableView_->currentIndex().row(),
					   AlarmsQueryModel::Act)).toString();

    ImagesDialog *dlg = new ImagesDialog(str, this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
  }
}


// void AlarmsWindow::readEvents(const QString &filename)
// {
//   QFile file(filename);
//   if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//     QMessageBox::warning(0, trUtf8("Ошибка"),
// 			 trUtf8("Не найден файл events.txt !!"));
//     return;
//   }

//   QTextStream in(&file);
//   while (!in.atEnd()) {
//     QString line = in.readLine();
//     QStringList fields = line.split(';');
//     if(fields.size() == 4) {
//       qint16 id    = fields.takeFirst().toInt();
//       qint8 type   = fields.takeFirst().toInt();
//       bool alert   = fields.takeFirst().toInt();
//       QString text = fields.takeFirst();
//       Event event  = { id, type, alert, text };
//       events_.push_back(event);
//     }
//   }

//   if(events_.size() == 0) {
//     QMessageBox::warning(0, trUtf8("Ошибка"), trUtf8("Ничего не загружено из файла events.txt !!"));
//     return;
//   }

//   std::sort(events_.begin(), events_.end());
// }
