#include <QtGui>

#include "MainWindow.h"
#include "AlarmsWindow.h"
#include "AlarmsLogDialog.h"
#include "PasswordDialog.h"
#include "EventsDialog.h"
#include "CameraDialog.h"
#include "ImagesDialog.h"

MainWindow::MainWindow()
  : valid_(true),
    isMightyUser_(true)
{
    alarmsWindow_ = new AlarmsWindow;
    setCentralWidget(alarmsWindow_);

    connect(alarmsWindow_, SIGNAL(newMailMsg(const QString&)),
	    this, SLOT(newMail(const QString&)));

    if(!alarmsWindow_->isValid())
      valid_ = false;

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    //readSettings();

    cameraDialog_ = 0;
    imagesDialog_ = 0;

    //setWindowIcon(QIcon(":/images/icon.png"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if(isMightyUser_) {
    writeSettings();
    event->accept();
  }
  else {
    int r = QMessageBox::warning(this, trUtf8("Подтверждение"),
        trUtf8("Действительно выйти?"),
        QMessageBox::Yes,
        QMessageBox::No | QMessageBox::Default | QMessageBox::Escape);

  if (r == QMessageBox::Yes)
    event->accept();
  else 
    event->ignore();
  }
}

/*
void MainWindow::find()
{
    if (!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(const QString &,
                                            Qt::CaseSensitivity)),
                spreadsheet, SLOT(findNext(const QString &,
                                           Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(const QString &,
                                                Qt::CaseSensitivity)),
                spreadsheet, SLOT(findPrevious(const QString &,
                                               Qt::CaseSensitivity)));
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}
*/

void MainWindow::password()
{
  PasswordDialog dialog(this);
  if(dialog.exec() == QDialog::Accepted) {
    isMightyUser_ = true;
  }
  else
    isMightyUser_ = false;

  eventsAction->setEnabled(isMightyUser_);
  passwordAction->setChecked(isMightyUser_);
}


void MainWindow::cameras()
{
  if(!cameraDialog_) {
    cameraDialog_ = new CameraDialog(this);
  }
  cameraDialog_->show();
  cameraDialog_->raise();
  cameraDialog_->activateWindow();
}

void MainWindow::newMail(const QString& str)
{
  // delete previous images
  QDir dir("./images/" + str);
  if(dir.exists()) {
    QStringList filters;
    foreach(QByteArray format, QImageReader::supportedImageFormats())
      filters += "*." + format;

    foreach(QString file, dir.entryList(filters, QDir::Files))
      QFile::remove(dir.path() + "/" + file);
  }
  else {
    QDir("./images").mkdir(str);
  }

  QProcess proc;
  QStringList arg;

  QSettings settings("tandem.conf", QSettings::IniFormat);
  QString path2spool = settings.value("path2spool",
				      "/var/spool/ss4lad").toString();
  if(!path2spool.endsWith('/'))
    path2spool.append('/');
  
  arg << "-f" <<  path2spool + str
      << "-d" << dir.path();
  proc.start("./mimeParser", arg);
  proc.waitForFinished();
  
  ImagesDialog *dlg = new ImagesDialog(str, this);
  dlg->setAttribute(Qt::WA_DeleteOnClose);
  dlg->show();
  
  // if(!imagesDialog_)
  //   imagesDialog_ = new ImagesDialog(str, this);

  // imagesDialog_->reload();
  // imagesDialog_->show();
  // imagesDialog_->raise();
  // imagesDialog_->activateWindow();
}

void MainWindow::imagesDialog()
{
  // if(!imagesDialog_)
  //   imagesDialog_ = new ImagesDialog(QString("./images"), this);

  // imagesDialog_->show();
  // imagesDialog_->raise();
  // imagesDialog_->activateWindow();
}


void MainWindow::events()
{
  if(isMightyUser_) {
    EventsDialog dialog(this);
    dialog.exec();
  }
}

void MainWindow::logs()
{
  if(isMightyUser_) {
    AlarmsLogDialog dialog(this);
    dialog.exec();
  }
}

void MainWindow::about()
{
    // QMessageBox::about(this, tr("About Spreadsheet"),
    //         tr("<h2>Spreadsheet 1.1</h2>"
    //            "<p>Copyright &copy; 2008 Software Inc."
    //            "<p>Spreadsheet is a small application that "
    //            "demonstrates QAction, QMainWindow, QMenuBar, "
    //            "QStatusBar, QTableWidget, QToolBar, and many other "
    //            "Qt classes."));
}


void MainWindow::updateStatusBar()
{
  locationLabel->setText(tr(""));
  formulaLabel->setText(tr(""));
}

/*
void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}
*/


void MainWindow::createActions()
{
    exitAction = new QAction(trUtf8("Выход"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(trUtf8("Выйти из программы"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    passwordAction = new QAction(trUtf8("Привелегированный режим"), this);
    passwordAction->setStatusTip(trUtf8("Привелегированный режим"));
    passwordAction->setCheckable(true);
    passwordAction->setChecked(isMightyUser_);
    connect(passwordAction, SIGNAL(triggered()), this, SLOT(password()));

    camerasAction = new QAction(trUtf8("Видеокамеры"), this);
    //limitsAction->setIcon(QIcon(":/images/find.png"));
    //findAction->setShortcut(QKeySequence::Find);
    camerasAction->setStatusTip(trUtf8("Камера"));
    connect(camerasAction, SIGNAL(triggered()), this, SLOT(cameras()));

    imagesAction = new QAction(trUtf8("Фотографии"), this);
    imagesAction->setStatusTip(trUtf8("Фотографии"));
    connect(imagesAction, SIGNAL(triggered()), this, SLOT(imagesDialog()));

    eventsAction = new QAction(trUtf8("Справочник событий"), this);
    eventsAction->setStatusTip(trUtf8("Справочник событий"));
    eventsAction->setEnabled(isMightyUser_);
    connect(eventsAction, SIGNAL(triggered()), this, SLOT(events()));

    logsAction = new QAction(trUtf8("Журнал событий"), this);
    logsAction->setStatusTip(trUtf8("Журнал событий за выбранный период"));
    logsAction->setEnabled(isMightyUser_);
    connect(logsAction, SIGNAL(triggered()), this, SLOT(logs()));

    
    // storagesAction = new QAction(trUtf8("&Склад"), this);
    // storagesAction->setStatusTip(trUtf8("Склад"));
    // connect(storagesAction, SIGNAL(triggered()), this, SLOT(storages()));

    // paysAction = new QAction(trUtf8("&Платежи"), this);
    // paysAction->setStatusTip(trUtf8("Список платежей"));
    // connect(paysAction, SIGNAL(triggered()), this, SLOT(pays()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(trUtf8("&Файл"));
    fileMenu->addAction(passwordAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(trUtf8("&Редактирование"));

    //editMenu->addSeparator();
    
    //editMenu->addAction(goToCellAction);

    toolsMenu = menuBar()->addMenu(trUtf8("&Инструменты"));
    toolsMenu->addAction(camerasAction);
    toolsMenu->addAction(imagesAction);
    toolsMenu->addAction(eventsAction);
    toolsMenu->addAction(logsAction);

    //reportsMenu = menuBar()->addMenu(trUtf8("&Отчеты"));
    //optionsMenu->addAction(showGridAction);
    //optionsMenu->addAction(autoRecalcAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(trUtf8("&Помощь"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createContextMenu()
{
    // spreadsheet->addAction(cutAction);
    // spreadsheet->addAction(copyAction);
    // spreadsheet->addAction(pasteAction);
    // spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    // fileToolBar = addToolBar(tr("&File"));
    // fileToolBar->addAction(newAction);
    // fileToolBar->addAction(openAction);
    // fileToolBar->addAction(saveAction);

    // editToolBar = addToolBar(tr("&Edit"));
    // editToolBar->addAction(cutAction);
    // editToolBar->addAction(copyAction);
    // editToolBar->addAction(pasteAction);
    // editToolBar->addSeparator();
    // editToolBar->addAction(findAction);
    // editToolBar->addAction(goToCellAction);
}

void MainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

    // connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
    //         this, SLOT(updateStatusBar()));
    // connect(spreadsheet, SIGNAL(modified()),
    //         this, SLOT(spreadsheetModified()));

    updateStatusBar();
}

void MainWindow::readSettings()
{
    // QSettings settings("Software Inc.", "Spreadsheet");

    // restoreGeometry(settings.value("geometry").toByteArray());

    // recentFiles = settings.value("recentFiles").toStringList();
    // updateRecentFileActions();

    // bool showGrid = settings.value("showGrid", true).toBool();
    // showGridAction->setChecked(showGrid);

    // bool autoRecalc = settings.value("autoRecalc", true).toBool();
    // autoRecalcAction->setChecked(autoRecalc);
}

void MainWindow::writeSettings()
{
    // QSettings settings("Software Inc.", "Spreadsheet");

    // settings.setValue("geometry", saveGeometry());
    // settings.setValue("recentFiles", recentFiles);
    // settings.setValue("showGrid", showGridAction->isChecked());
    // settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

