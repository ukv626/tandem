#ifndef EVENTSDIALOG_H
#define EVENTSDIALOG_H

#include <QDialog>
#include <QSqlRelationalTableModel> 
class QTableView;

class EventsRelModel : public QSqlRelationalTableModel {
     Q_OBJECT
 public:
  enum { Id, Text, Type, IsAlert, IsVisible };
  
  EventsRelModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
};

class EventsDialog : public QDialog
{
    Q_OBJECT
public:
  EventsDialog(QWidget *parent = 0);
  ~EventsDialog();

private slots:
  void insert();
  void remove();

private:
  //QLabel *label;
  //QLineEdit *lineEdit;
  //QComboBox *comboBox;
  QTableView *tableView_;
  EventsRelModel *tableModel_;
};

#endif
