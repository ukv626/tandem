#include <QtGui>
#include <QtSql>
#include <QTcpServer>
#include <QTcpSocket>
#include <algorithm>

#include "alarmswindow.h"
#include "ImagesDialog.h"

AlarmsTableView::AlarmsTableView(QWidget *parent)
  : QTableView(parent)
{
}

void AlarmsTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
  emit(doubleClicked(this->indexAt(QPoint(event->pos().x(), event->pos().y()))));
  event->accept();
}


AlarmsRelTableModel::AlarmsRelTableModel(QObject *parent)
  : QSqlRelationalTableModel(parent)
{
}

QVariant AlarmsRelTableModel::data(const QModelIndex &index, int role) const
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

    
    //   case Qt::TextAlignmentRole: // Выравнивание
    //     if(index.column() == MoveDialog::Move_N ||
    // 	 index.column() == MoveDialog::Move_Ost )
    // 	return double(Qt::AlignRight | Qt::AlignVCenter);
    //     else
    // 	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}


// -- AlarmsWindow -----------------------------------------------
AlarmsWindow::AlarmsWindow(QWidget *parent)
  : QWidget(parent), nextBlockSize(0), valid_(true)
{
  label1_ = new QLabel(trUtf8("<H2>Пост 1</H2>"));
  label1_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label1_->setStyleSheet("background-color: green; border: 2px solid grey");
  
  label2_ = new QLabel(trUtf8("<H2>Пост 2</H2>"));
  label2_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label2_->setStyleSheet("background-color: green; border: 2px solid grey");
  
  label3_ = new QLabel(trUtf8("<H2>Пост 3</H2>"));
  label3_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label3_->setStyleSheet("background-color: red; border: 2px solid grey");

  label4_ = new QLabel(trUtf8("<H2>Пост 4</H2>"));
  label4_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label4_->setStyleSheet("background-color: yellow; border: 2px solid grey");
  
  tableView_ = new AlarmsTableView(this);

  tableModel_ = new AlarmsRelTableModel(this);
  tableModel_->setTable("tb_logs");
  tableModel_->setRelation(AlarmsRelTableModel::Eee,
			   QSqlRelation("tb_events", "id", "text"));
  tableModel_->setSort(AlarmsRelTableModel::Date, Qt::DescendingOrder);
  //tableModel_->setEditStrategy(QSqlTableModel::OnManualSubmit);
  tableModel_->select();

  tableModel_->setHeaderData(AlarmsRelTableModel::Date,
			     Qt::Horizontal, trUtf8("Дата"));
  tableModel_->setHeaderData(AlarmsRelTableModel::Act, Qt::Horizontal, trUtf8("ACT"));
  tableModel_->setHeaderData(AlarmsRelTableModel::Q, Qt::Horizontal, trUtf8("Q"));
  tableModel_->setHeaderData(AlarmsRelTableModel::Eee, Qt::Horizontal, trUtf8("EEE"));
  tableModel_->setHeaderData(AlarmsRelTableModel::Gg, Qt::Horizontal, trUtf8("GG"));
  tableModel_->setHeaderData(AlarmsRelTableModel::Zzz, Qt::Horizontal, trUtf8("ZZZ"));

  tableView_->setModel(tableModel_);
  tableView_->setItemDelegate(new QSqlRelationalDelegate(tableView_));

  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  
  tableView_->setColumnHidden(AlarmsRelTableModel::IsRead, true);
  tableView_->setColumnHidden(AlarmsRelTableModel::Id, true);
  // tableView->setColumnHidden(MoveDialog::Move_Qty, true);
  
  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  tableView_->setAlternatingRowColors(true);
  //tableView_->setCurrentIndex(tableView_->model()->index(0, 0));
  tableView_->selectRow(0);

  QAction *showMessageAction = new QAction(trUtf8("Просмотр.."), this);
  //showMessageAction->setShortcut(tr("Ins"));
  connect(showMessageAction, SIGNAL(triggered()), this, SLOT(showMessage()));

  tableView_->addAction(showMessageAction);
  tableView_->setContextMenuPolicy(Qt::ActionsContextMenu);

  connect(tableView_, SIGNAL(doubleClicked(QModelIndex)),
  	  this, SLOT(doubleClick(QModelIndex)));

  connect(tableView_->selectionModel(),
  	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  	  SLOT(showPictures(const QItemSelection &, const QItemSelection &))
  	  );
  
  textBrowser_ = new QTextBrowser();

  textBrowser_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  textBrowser_->setFixedWidth(420);
  
  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(label1_);
  topLayout->addWidget(label2_);
  topLayout->addWidget(label3_);
  topLayout->addWidget(label4_);

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

  tcpServer_ = new QTcpServer(this);

  if (!tcpServer_->listen(QHostAddress::Any, 45000)) {
    QMessageBox::critical(0, trUtf8("Ошибка"), tcpServer_->errorString());
    valid_ = false;
  }
  else
    connect(tcpServer_, SIGNAL(newConnection()), this, SLOT(accept()));
}

AlarmsWindow::~AlarmsWindow()
{
  delete tableModel_;
  delete tcpServer_;
}

void AlarmsWindow::doubleClick(const QModelIndex &index)
{
  int row = index.row();
  QAbstractItemModel *model = tableView_->model();
  QModelIndex ind = model->index(row, AlarmsRelTableModel::IsRead);
  model->setData(ind, model->data(ind).toInt() == 0 ? 1 : 0);
  model->submit();
  tableView_->selectRow(row);
}

bool AlarmsWindow::newEvent()
{
  qDebug() << data_; //.mid(0,3).toHex().toUpper()
  
  QString msgType = data_.mid(8, 7);    
  if(msgType != "ADM-CID" && msgType != "NEW-MSG") 
    return false;

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

  data_.remove(leftBrInd + 1, rightBrInd - leftBrInd - 1);
  data_.replace(msgType, QByteArray("ACK"));
    
  data_.replace(3, 4, QString("%1").arg(data_.size() - 8, 4, 16,
					QChar('0')).toUpper().toAscii());

  quint8 row = 0;
  QAbstractItemModel *model = tableView_->model();
  model->insertRow(row);
  model->setData(model->index(row, AlarmsRelTableModel::Date),
  		 QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
  model->setData(model->index(row, AlarmsRelTableModel::Act), msgAct);
  
  qint8 q = msgOptTail.left(1).toInt();
  model->setData(model->index(row, AlarmsRelTableModel::Q), q);
  
  qint16 eee = msgOptTail.mid(1, 3).toInt();
  model->setData(model->index(row, AlarmsRelTableModel::Eee),
		 q != 3 ? eee : eee*(-1));
  model->setData(model->index(row, AlarmsRelTableModel::Gg),
		 msgOptTail.mid(5, 2).toInt());
  model->setData(model->index(row, AlarmsRelTableModel::Zzz),
		 msgOptTail.right(3).toInt());
  model->submit();
  
  if(msgType == "NEW-MSG")
    emit newMailMsg(msgAct);

  return true;
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
      qDebug() << data_;
    
    nextBlockSize = 0;
    data_.truncate(0);
  }
}

void AlarmsWindow::showPictures(const QItemSelection &selected,
				const QItemSelection &/* deselected */)
{
  if(selected.size() == 0)
    return;

  QAbstractItemModel *model = tableView_->model();
  QModelIndexList indexes = selected.indexes();
  QString str = model->data(indexes[2]).toString();

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
					AlarmsRelTableModel::Eee),
			   Qt::EditRole).toString();

  if(eee == trUtf8("НОВОЕ СООБЩЕНИЕ")) {
    QString str = model->data(model->index(tableView_->currentIndex().row(),
					   AlarmsRelTableModel::Act)).toString();

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
