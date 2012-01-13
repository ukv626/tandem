#ifndef IMAGESDIALOG_H
#define IMAGESDIALOG_H

#include <QDialog>

class QTextBrowser;
class QDialogButtonBox;

class ImagesDialog : public QDialog
{
    Q_OBJECT
public:
  ImagesDialog(const QString &dir, QWidget *parent = 0);
  ~ImagesDialog();
  void reload();

private:
  const QString dir_;
  QTextBrowser *textBrowser_;
  QDialogButtonBox *buttonBox_;
};

#endif
