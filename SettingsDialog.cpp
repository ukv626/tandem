#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QSettings>
#include <QRegExpValidator>
#include <QDialogButtonBox>
#include <QDebug>

#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
  : QDialog(parent)
{
  QString octet = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QSettings settings("./tandem.conf", QSettings::IniFormat);
  
  portEdit = new QLineEdit(settings.value("port", "45002").toString());
  portEdit->setValidator(new QIntValidator(this));
  portEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  portEdit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  portLabel = new QLabel(trUtf8("<HTML>Порт<BR><FONT size=\"-2\">(требуется перезапуск)</FONT></HTML>"));
  portLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  portLabel->setBuddy(portEdit);

  
  camIP1Edit = new QLineEdit(settings.value("camera1", "127.0.0.1").toString());
  camIP1Edit->setValidator(new QRegExpValidator(QRegExp("^" + octet + "\\." +
			octet + "\\." + octet + "\\." + octet + "$"), this));
  camIP1Edit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  camIP1Label = new QLabel(trUtf8("Регистратор №1 IP"));
  camIP1Label->setBuddy(camIP1Edit);

  camIP2Edit = new QLineEdit(settings.value("camera2", "127.0.0.1").toString());
  camIP2Edit->setValidator(new QRegExpValidator(QRegExp("^" + octet + "\\." +
			octet + "\\." + octet + "\\." + octet + "$"), this));
  camIP2Edit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  camIP2Label = new QLabel(trUtf8("Регистратор №2 IP"));
  camIP2Label->setBuddy(camIP2Edit);

  camIP3Edit = new QLineEdit(settings.value("camera3", "127.0.0.1").toString());
  camIP3Edit->setValidator(new QRegExpValidator(QRegExp("^" + octet + "\\." +
			octet + "\\." + octet + "\\." + octet + "$"), this));
  camIP3Edit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  camIP3Label = new QLabel(trUtf8("Регистратор №3 IP"));
  camIP3Label->setBuddy(camIP3Edit);

  camFPSEdit = new QSpinBox;
  camFPSEdit->setRange(1,25);
  camFPSEdit->setValue(settings.value("fps", "15").toInt());
  camFPSEdit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  camFPSLabel = new QLabel(trUtf8("Кадров/сек"));
  camFPSLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  camFPSLabel->setBuddy(camFPSEdit);

  mainGroup = new QGroupBox(trUtf8("Основные"));
  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->addWidget(portLabel, 0, 0);
  mainLayout->addWidget(portEdit, 0, 1);
  mainGroup->setLayout(mainLayout);
  
  videoGroup = new QGroupBox(trUtf8("Видео"));
  QGridLayout *videoLayout = new QGridLayout;
  videoLayout->addWidget(camIP1Label, 0, 0);
  videoLayout->addWidget(camIP1Edit, 0, 1);
  videoLayout->addWidget(camIP2Label, 1, 0);
  videoLayout->addWidget(camIP2Edit, 1, 1);
  videoLayout->addWidget(camIP3Label, 2, 0);
  videoLayout->addWidget(camIP3Edit, 2, 1);
  videoLayout->addWidget(camFPSLabel, 3, 0);
  videoLayout->addWidget(camFPSEdit, 3, 1);
  videoGroup->setLayout(videoLayout);
  

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(mainGroup);
  layout->addWidget(videoGroup);
  layout->addWidget(buttonBox);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);
  

  setWindowTitle(trUtf8("Настройки"));
}


SettingsDialog::~SettingsDialog()
{
  //delete queryModel;
  //delete proxyModel;
}




void SettingsDialog::ok()
{
  QSettings settings("./tandem.conf", QSettings::IniFormat);
  if(portEdit->isModified())
    settings.setValue("port", portEdit->text());

  if(camIP1Edit->isModified())
    settings.setValue("camera1", camIP1Edit->text());

  if(camIP2Edit->isModified())
    settings.setValue("camera2", camIP2Edit->text());

  if(camIP3Edit->isModified())
    settings.setValue("camera3", camIP3Edit->text());

  if(settings.value("fps", "15").toInt() != camFPSEdit->value())
    settings.setValue("fps", camFPSEdit->value());
    
  accept();
}
