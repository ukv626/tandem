#include <QtGui>

#include "mainwindow.h"
#include "alarmswindow.h"
// #include "requestsdialog.h"
// #include "storagesdialog.h"
#include "CameraDialog.h"
#include "ImagesDialog.h"

MainWindow::MainWindow()
{
    alarmsWindow_ = new AlarmsWindow;
    setCentralWidget(alarmsWindow_);

    connect(alarmsWindow_, SIGNAL(newMailMsg()),
	    this, SLOT(cameras()));

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    //readSettings();

    cameraDialog_ = 0;

    //setWindowIcon(QIcon(":/images/icon.png"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  writeSettings();
  event->accept();
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

void MainWindow::cameras()
{
  if(!cameraDialog_) {
    cameraDialog_ = new CameraDialog(this);
  }
  cameraDialog_->show();
  cameraDialog_->raise();
  cameraDialog_->activateWindow();
}

void MainWindow::imagesDialog()
{
  ImagesDialog dialog(this);
  dialog.exec();
}


void MainWindow::storages()
{
  // StoragesDialog dialog(this);
  // dialog.exec();
}

void MainWindow::pays()
{
  // PaysDialog dialog(this);
  // dialog.exec();
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

    camerasAction = new QAction(trUtf8("Видеокамеры"), this);
    //limitsAction->setIcon(QIcon(":/images/find.png"));
    //findAction->setShortcut(QKeySequence::Find);
    camerasAction->setStatusTip(trUtf8("Камера"));
    connect(camerasAction, SIGNAL(triggered()), this, SLOT(cameras()));

    imagesAction = new QAction(trUtf8("Фотографии"), this);
    imagesAction->setStatusTip(trUtf8("Фотографии"));
    connect(imagesAction, SIGNAL(triggered()), this, SLOT(imagesDialog()));
    
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
    //fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(trUtf8("&Редактирование"));

    //editMenu->addSeparator();
    
    //editMenu->addAction(goToCellAction);

    toolsMenu = menuBar()->addMenu(trUtf8("&Инструменты"));
    toolsMenu->addAction(camerasAction);
    toolsMenu->addAction(imagesAction);
    //toolsMenu->addAction(sortAction);

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

