#ifndef ALARMSWINDOW_H
#define ALARMSWINDOW_H

#include <QWidget>
#include <QByteArray>
#include <vector>
#include <QSqlRelationalTableModel>
#include <QTableView>

class QLabel;
class QTcpServer;
class QTableWidget;

class AlarmsTableView: public QTableView
{
  Q_OBJECT
 public:
  AlarmsTableView(QWidget *parent);
 signals:
  void doubleClicked (const QModelIndex & index);
 private:
  void mouseDoubleClickEvent(QMouseEvent * event);
};

/* struct Event */
/* { */
/*   qint16 id; */
/*   qint8 type; */
/*   bool alert; */
/*   QString text; */

/*   bool operator<(const Event &event) const */
/*   { */
/*     return id < event.id; */
/*   } */

/* }; */

/* struct compareEventId */
/* { */
/*   compareEventId(qint16 id) */
/*     :id_(id) */
/*   {} */
/*   bool operator()(const Event &event) */
/*   { */
/*     return id_ == event.id; */
/*   } */

/* private: */
/*   qint16 id_; */
/* }; */


class AlarmsRelTableModel : public QSqlRelationalTableModel {
     Q_OBJECT
 public:
  enum {
    Date = 0,
    Act = 1,
    Q = 2,
    Eee = 3,
    Gg = 4,
    Zzz = 5,
    IsRead = 6
  };
  
  AlarmsRelTableModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
};


// -- AlarmsWindow -----------------------------------------------
class AlarmsWindow : public QWidget
{
    Q_OBJECT

public:
  enum { HeaderLength = 7 };
  AlarmsWindow(QWidget *parent = 0);
  ~AlarmsWindow();
  
  bool newEvent();
  bool isValid() { return valid_; }

private slots:
  void accept();
  void readMsg();
  void showMessage();
  void doubleClick(const QModelIndex &index);

signals:
  void newMailMsg(const QString& str);

private:
  void readEvents(const QString &filename);
  AlarmsTableView *tableView_;
  AlarmsRelTableModel *tableModel_;
    
  QTcpServer *tcpServer_;
  QByteArray data_;
  quint8 nextBlockSize;
  QLabel *label1_, *label2_, *label3_, *label4_;
  /* std::vector<Event> events_; */
  bool valid_;
    /* QLineEdit *lineEdit; */
};



#endif
