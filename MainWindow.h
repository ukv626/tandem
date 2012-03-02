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
    void password();
    void post1Video();
    void post2Video();
    void post3Video();
    void post4Video();
    void post5Video();
    void newMail(const QString& str);
    void imagesDialog();
    void events();
    void logs();
    /* void pays(); */
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
    ImagesDialog *imagesDialog_;
    QLabel *locationLabel;
    QLabel *formulaLabel;

    QMenu *fileMenu;
    QMenu *videoMenu;
    QMenu *toolsMenu;
    QMenu *reportsMenu;
    QMenu *helpMenu;
    
    //QToolBar *fileToolBar;
    //QToolBar *editToolBar;

    QAction *exitAction;
    QAction *passwordAction;
    QAction *post1Action;
    QAction *post2Action;
    QAction *post3Action;
    QAction *post4Action;
    QAction *post5Action;
    /* QAction *imagesAction; */
    QAction *eventsAction;
    QAction *logsAction; 
    /* QAction *storagesAction; */
    /* QAction *paysAction; */
    QAction *aboutAction;
    bool valid_;
    bool isMightyUser_;
};

#endif
