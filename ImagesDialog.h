#ifndef IMAGESDIALOG_H
#define IMAGESDIALOG_H

#include <QDialog>

class QTextBrowser;
class QDialogButtonBox;

class ImagesDialog : public QDialog
{
    Q_OBJECT
public:
    ImagesDialog(QWidget *parent = 0);
    

private:
    QTextBrowser *textBrowser_;
    QDialogButtonBox *buttonBox_;
};

#endif
