#include <QLabel>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QCryptographicHash>
#include <QDebug>

#include "PasswordDialog.h"

PasswordDialog::PasswordDialog(QWidget *parent)
  : QDialog(parent)
{
  password_ = new QLineEdit;
  label_ = new QLabel(trUtf8("Пароль"));
  label_->setBuddy(password_);

  buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);

  connect(buttonBox_, SIGNAL(accepted()), this, SLOT(ok()));
  connect(buttonBox_, SIGNAL(rejected()), this, SLOT(reject()));

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(label_);
  topLayout->addWidget(password_);
  
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(buttonBox_);
  setLayout(mainLayout);

  setWindowTitle(tr("PasswordDialog"));
}

void PasswordDialog::ok()
{
  QCryptographicHash hasher(QCryptographicHash::Sha1);
  hasher.addData(password_->text().toAscii());
  
  if(hasher.result().toHex() == "ec307432a3d742bc70041ab01f6740a57f34ba53")
    accept();
  else {
    password_->setText("");
    password_->setFocus();
  }
  

  // QCryptographicHash hasher(QCryptographicHash::Sha1);
  // hasher.addData("Nokia");
  // QByteArray string1=hasher.result().toHex();
  // qDebug() << string1;
}
