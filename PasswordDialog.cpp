#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QCryptographicHash>
#include <QDebug>

#include "PasswordDialog.h"

PasswordDialog::PasswordDialog(QWidget *parent)
  : QDialog(parent)
{
  password_ = new QLineEdit;
  password_->setEchoMode(QLineEdit::Password);
  label_ = new QLabel(trUtf8("Пароль"));
  label_->setBuddy(password_);

  okButton_ = new QPushButton(trUtf8("ОК"));
  cancelButton_ = new QPushButton(trUtf8("Отмена"));

  buttonBox_ = new QDialogButtonBox;
  buttonBox_->addButton(okButton_, QDialogButtonBox::AcceptRole);
  buttonBox_->addButton(cancelButton_, QDialogButtonBox::RejectRole);

  connect(buttonBox_, SIGNAL(accepted()), this, SLOT(ok()));
  connect(buttonBox_, SIGNAL(rejected()), this, SLOT(reject()));

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(label_);
  topLayout->addWidget(password_);
  
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(buttonBox_);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Введите пароль"));
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
