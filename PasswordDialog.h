#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QDialogButtonBox;
class QPushButton;

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
  PasswordDialog(QWidget *parent);

private slots:
  void ok();
  
private:
  QLabel *label_;
  QLineEdit *password_;

  QDialogButtonBox *buttonBox_;
  QPushButton *okButton_;
  QPushButton *cancelButton_;
  
};

#endif
