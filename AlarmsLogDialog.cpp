#include <QtGui>
#include <QtSql>

#include "AlarmsLogDialog.h"

AlarmsLogQueryModel::AlarmsLogQueryModel(QObject *parent)
  : QSqlQueryModel(parent)
{
  refresh(QDate::currentDate(), QDate::currentDate());
}

QVariant AlarmsLogQueryModel::data(const QModelIndex &index, int role) const
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

void AlarmsLogQueryModel::refresh(const QDate& date1, const QDate& date2)
{
  QSqlQuery query;
  query.prepare("SELECT l.id, l.date_, l.act, l.q, e.text, l.gg, l.zzz "
	   ",l.isRead,e.type,e.isAlert "
	   "FROM tb_logs l "
	   ",tb_events e "
	   "WHERE l.eee=e.id "
	   "AND DATE(l.date_)>=:date1 "
	   "AND DATE(l.date_)<=:date2 "
	   "ORDER BY l.date_ DESC");
  query.bindValue(":date1", date1);
  query.bindValue(":date2", date2);
  query.exec();

  setQuery(query);
  setHeaderData(Date,
  		Qt::Horizontal, trUtf8("Дата"));
  setHeaderData(Act, Qt::Horizontal, trUtf8("ACT"));
  setHeaderData(Q, Qt::Horizontal, trUtf8("Q"));
  setHeaderData(Eee, Qt::Horizontal, trUtf8("EEE"));
  setHeaderData(Gg, Qt::Horizontal, trUtf8("GG"));
  setHeaderData(Zzz, Qt::Horizontal, trUtf8("ZZZ"));
}


// -- AlarmsWindow -----------------------------------------------
AlarmsLogDialog::AlarmsLogDialog(QWidget *parent)
  : QDialog(parent)
{
  date1Edit_ = new QDateEdit(QDate::currentDate());
  date1Edit_->setCalendarPopup(true);
  date1Edit_->setDisplayFormat("dd.MM.yyyy");
  date1Label_ = new QLabel(trUtf8("Дата"));
  date1Label_->setBuddy(date1Edit_);

  date2Edit_ = new QDateEdit(QDate::currentDate());
  date2Edit_->setCalendarPopup(true);
  date2Edit_->setDisplayFormat("dd.MM.yyyy");
  date2Label_ = new QLabel(trUtf8("Дата"));
  date2Label_->setBuddy(date2Edit_);

  connect(date1Edit_, SIGNAL(dateChanged(const QDate&)), this,
	  SLOT(date1Changed(const QDate&)));
  connect(date2Edit_, SIGNAL(dateChanged(const QDate&)), this,
	  SLOT(date2Changed(const QDate&)));

  tableView_ = new QTableView(this);
  
  tableModel_ = new AlarmsLogQueryModel(this);
  
  tableView_->setModel(tableModel_);
  // tableView_->setItemDelegate(new QSqlRelationalDelegate(tableView_));

  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  tableView_->setColumnHidden(AlarmsLogQueryModel::Id, true);
  tableView_->setColumnHidden(AlarmsLogQueryModel::Q, true);
  tableView_->setColumnHidden(AlarmsLogQueryModel::IsRead, true);
  
  // tableView->setColumnHidden(MoveDialog::Move_Qty, true);
  
  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  tableView_->setAlternatingRowColors(true);
  //tableView_->setCurrentIndex(tableView_->model()->index(0, 0));
  tableView_->selectRow(0);

  // QGridLayout *topLayout = new QGridLayout;
  // topLayout->addWidget(date1Label_, 1, 0);
  // topLayout->addWidget(date1Edit_, 1, 1);
  // topLayout->addWidget(date2Label_, 2, 0);
  // topLayout->addWidget(date2Edit_, 2, 1);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(date1Edit_);
  topLayout->addWidget(date2Edit_);
  
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(topLayout);
  layout->addWidget(tableView_);
  
  // topLayout->addWidget(label_);
  // topLayout->addStretch();
  // topLayout->addWidget(messagesTable_);
    
  // QVBoxLayout *mainLayout = new QVBoxLayout; 
  // mainLayout->addLayout(topLayout);
  // mainLayout->addWidget(tableView);
  setLayout(layout);
 
  setWindowTitle(tr("AlarmsLog"));
  setFixedWidth(tableView_->horizontalHeader()->length()+50);
  setFixedHeight(500);
}

AlarmsLogDialog::~AlarmsLogDialog()
{
  delete tableModel_;
}


void AlarmsLogDialog::date1Changed(const QDate &date)
{
  tableModel_->refresh(date, date2Edit_->date());
  tableView_->selectRow(0);
}

void AlarmsLogDialog::date2Changed(const QDate &date)
{
  tableModel_->refresh(date1Edit_->date(), date);
  tableView_->selectRow(0);
}
