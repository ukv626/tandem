#include <QtGui>

#include "ImagesDialog.h"

ImagesDialog::ImagesDialog(QWidget *parent)
    : QDialog(parent)
{
  textBrowser_ = new QTextBrowser();
  QString page("<html>");

  QDir dir;
  QStringList filters;
  foreach(QByteArray format, QImageReader::supportedImageFormats())
    filters += "*." + format;

  foreach(QString file, dir.entryList(filters, QDir::Files))
    page += tr("<img src=\"%1\" width=352 height=288 />").arg(file);
  
  page += "</html>";
  
  textBrowser_->setHtml(page);
  //label->setPixmap(QPixmap("image2.jpg"));


  // startButton = new QPushButton(tr("start"));
  // startButton->setDefault(true);
  // stopButton = new QPushButton(tr("stop"));
  // closeButton = new QPushButton(tr("close"));

  buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok
				   | QDialogButtonBox::Cancel);
  // buttonBox->addButton(startButton, QDialogButtonBox::AcceptRole);
  // buttonBox->addButton(stopButton, QDialogButtonBox::ActionRole);
  // buttonBox->addButton(closeButton,QDialogButtonBox::RejectRole);
  
  connect(buttonBox_, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox_, SIGNAL(rejected()), this, SLOT(reject()));

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(textBrowser_);
  layout->addWidget(buttonBox_);
  setLayout(layout);

  setWindowTitle(trUtf8("Images"));
  resize(800, 600);
  // setFixedHeight(sizeHint().height());
  // setFixedWidth(sizeHint().width());
  
}



