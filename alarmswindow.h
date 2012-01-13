#ifndef ALARMSWINDOW_H
#define ALARMSWINDOW_H

#include <QWidget>
#include <QByteArray>
#include <vector>

class QLabel;
/* class QLineEdit; */
class QTableWidget;
class QTcpServer;

struct Event
{
  qint16 id;
  qint8 type;
  bool alert;
  QString text;

  bool operator<(const Event &event) const
  {
    return id < event.id;
  }

};

struct compareEventId
{
  compareEventId(qint16 id)
    :id_(id)
  {}
  bool operator()(const Event &event)
  {
    return id_ == event.id;
  }

private:
  qint16 id_;
};


// -- AlarmsWindow -----------------------------------------------
class AlarmsWindow : public QWidget
{
    Q_OBJECT

public:
  enum { HeaderLength = 7 };
  AlarmsWindow(QWidget *parent = 0);
  void newEvent(QByteArray &data);
  void newMessage(const QString &file);
  bool isValid() { return valid_; }

private slots:
  void accept();
  void readMsg();
  void showMessage();
  

signals:
  void newMailMsg(const QString& str);
  void aborted();

private:
  void readEvents(const QString &filename);
  /* QLabel *label_; */
  QTableWidget *messagesTable_;
  QTableWidget *eventsTable_;
    
  QTcpServer *tcpServer_;
  QByteArray data_;
  quint8 nextBlockSize;
  std::vector<Event> events_;
  bool valid_;
    /* QLineEdit *lineEdit; */
};

#endif
