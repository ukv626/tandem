#ifndef ALARMSWINDOW_H
#define ALARMSWINDOW_H

#include <QWidget>
#include <QByteArray>
#include <vector>
#include <QSqlQueryModel>
#include <QTableView>
#include <QVector>

class QLabel;
class QTcpServer;
class QTableWidget;
class QTextBrowser;

class AlarmsTableView: public QTableView
{
  Q_OBJECT
 public:
  AlarmsTableView(QWidget *parent);
 signals:
  void doubleClicked(const QModelIndex &index);

 private:
  void mouseDoubleClickEvent(QMouseEvent *event);
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


class AlarmsQueryModel : public QSqlQueryModel {
     Q_OBJECT
 public:
  enum { Id, Date, Act, Q, Eee, Gg, Zzz, IsRead, Type, IsAlert };
  
  AlarmsQueryModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int /* role */);
  void refresh();
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
  void showPictures(const QModelIndex &current, const QModelIndex &previous);		  void showMessage();
  void doubleClick(const QModelIndex &index);

signals:
  void newMailMsg(const QString& str);

private:
  void readEvents(const QString &filename);
  QString getColorByType(int type);
  
  AlarmsTableView *tableView_;
  AlarmsQueryModel *tableModel_;
    
  QTcpServer *tcpServer_;
  QByteArray data_;
  quint8 nextBlockSize;
  QVector<QLabel *> labels;
  QTextBrowser *textBrowser_;
  /* std::vector<Event> events_; */
  bool valid_;
    /* QLineEdit *lineEdit; */
};



#endif
