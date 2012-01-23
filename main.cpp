#include <QtGui>
#include <QtSql>

#include "mainwindow.h"

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

    if (!createConnection())
      return 1;


    MainWindow mainWin;
    if(mainWin.isValid()) {
      mainWin.resize(900, 600);
      //mainWin.setWindowState(Qt::WindowMaximized);
      mainWin.show();

      return app.exec();
    }
    return -1;
}
