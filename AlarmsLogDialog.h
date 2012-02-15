#ifndef ALARMSLOGWINDOW_H
#define ALARMSLOGWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>

class QLabel;
class QTableView;
class QDateEdit;

class AlarmsLogQueryModel : public QSqlQueryModel {
     Q_OBJECT
 public:
  enum { Id, Date, Act, Q, Eee, Gg, Zzz, IsRead, Type, IsAlert };
  
  AlarmsLogQueryModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
  void refresh(const QDate& date1, const QDate& date2);
};


// -- AlarmsLogDialog -----------------------------------------------
class AlarmsLogDialog : public QDialog
{
    Q_OBJECT

public:
  AlarmsLogDialog(QWidget *parent = 0);
  ~AlarmsLogDialog();
  private slots:
  void date1Changed(const QDate &date);
  void date2Changed(const QDate &date);
  
private:
  QLabel *date1Label_;
  QLabel *date2Label_;
  
  QDateEdit *date1Edit_;
  QDateEdit *date2Edit_;
  
  QTableView *tableView_;
  AlarmsLogQueryModel *tableModel_;
};

#endif
