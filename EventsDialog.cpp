#include <QtGui>
#include <QtSql>

#include "EventsDialog.h"

EventsRelModel::EventsRelModel(QObject *parent)
  : QSqlRelationalTableModel(parent)
{
  setTable("tb_events");
  setRelation(Type, QSqlRelation("tb_eventsType", "id", "text"));
  setSort(Text, Qt::AscendingOrder);
  select();

  setHeaderData(Type, Qt::Horizontal, trUtf8("Тип"));
  setHeaderData(IsAlert, Qt::Horizontal, trUtf8("IsAlert"));
  setHeaderData(IsVisible, Qt::Horizontal, trUtf8("IsVisible"));
  setHeaderData(Text, Qt::Horizontal, trUtf8("Наименование"));
}

QVariant EventsRelModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlQueryModel::data(index, role);
  switch (role) {
      case Qt::TextAlignmentRole: // Выравнивание
        if(index.column() == Id ||
	   index.column() == IsAlert ||
	   index.column() == IsVisible)
  	  return int(Qt::AlignRight | Qt::AlignVCenter);
  	else
  	  return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}


EventsDialog::EventsDialog(QWidget *parent)
  : QDialog(parent)
{
  tableModel_ = new EventsRelModel;
  
  tableView_ = new QTableView;
  
  tableView_->setModel(tableModel_);
  tableView_->setItemDelegate(new QSqlRelationalDelegate(tableView_));

  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  
  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  tableView_->setAlternatingRowColors(true);

  tableView_->resizeColumnsToContents();
  tableView_->setCurrentIndex(tableView_->model()->index(0, 0));

  QAction *newAction = new QAction(trUtf8("Добавить.."), this);
  connect(newAction, SIGNAL(triggered()), this, SLOT(insert()));
  
  // QAction *removeAction = new QAction(trUtf8("Удалить"), this);
  // connect(removeAction, SIGNAL(triggered()), this, SLOT(remove()));

  tableView_->addAction(newAction);
  // tableView->addAction(removeAction);
  tableView_->setContextMenuPolicy(Qt::ActionsContextMenu);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView_);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Справочник событий"));
  setFixedWidth(tableView_->horizontalHeader()->length()+50);
  setFixedHeight(380);
}

EventsDialog::~EventsDialog()
{
  delete tableModel_;
}

void EventsDialog::insert()
{
  QItemSelectionModel *selection = tableView_->selectionModel();
  int row = selection->selectedIndexes().first().row();

  QAbstractItemModel *model = tableView_->model();
  QModelIndex index = model->index(row, EventsRelModel::Text);
  model->insertRow(row+1);

  tableView_->setCurrentIndex(index);
}

void EventsDialog::remove()
{
  //qDebug() << tableModel->lastError();
}
