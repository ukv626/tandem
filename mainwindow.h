#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class AlarmsWindow;
class CameraDialog;
class ImagesDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    bool isValid() { return valid_; }

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void cameras();
    void newMail(const QString& str);
    void imagesDialog();
    void storages();
    void pays();
    void about();
    void updateStatusBar();

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();

    AlarmsWindow *alarmsWindow_;
    CameraDialog *cameraDialog_;
    ImagesDialog *imagesDialog_;
    QLabel *locationLabel;
    QLabel *formulaLabel;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *reportsMenu;
    QMenu *helpMenu;
    
    //QToolBar *fileToolBar;
    //QToolBar *editToolBar;

    QAction *exitAction;
    QAction *camerasAction;
    QAction *imagesAction; 
    /* QAction *storagesAction; */
    /* QAction *paysAction; */
    QAction *aboutAction;
    bool valid_;
};

#endif
