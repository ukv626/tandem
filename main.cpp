#include <QtGui>
#include <QtSql>

#include "MainWindow.h"

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./data/data.db");

    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
        return false;
    }
    return true;
}


int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  
  QSettings settings("/etc/tandem.conf", QSettings::IniFormat);
  QDir::setCurrent(settings.value("dir", "./").toString());

  if (!createConnection())
    return 1;

  MainWindow mainWin;
  if(mainWin.isValid()) {
    //mainWin.resize(900, 600);
    // mainWin.setWindowState(Qt::WindowMaximized);
    
      // mainWin.setWindowFlags(Qt::FramelessWindowHint |
      // 			     Qt::Tool |
      // 			     Qt::WindowStaysOnTopHint);
      // mainWin.resize(QApplication::desktop()->size());
      
      // mainWin.setFocusPolicy( Qt::StrongFocus );
      // mainWin.setAttribute(Qt::WA_QuitOnClose, true);
    mainWin.showFullScreen();
    return app.exec();
  }
  return -1;
}
