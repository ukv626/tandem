#include <QtGui>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWin;
    if(mainWin.isValid()) {
      mainWin.resize(800, 600);
      //mainWin.setWindowState(Qt::WindowMaximized);
      mainWin.show();

      return app.exec();
    }
    return -1;
}
