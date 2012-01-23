#include <QtGui>

#include "ImagesDialog.h"

ImagesDialog::ImagesDialog(const QString &dir, QWidget *parent)
  : QDialog(parent),
    dir_(dir)
{
  label_ = new QLabel();
  textBrowser_ = new QTextBrowser();
  reload();
  
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
  layout->addWidget(label_);
  layout->addWidget(textBrowser_);
  layout->addWidget(buttonBox_);
  setLayout(layout);

  setWindowTitle(trUtf8("Images"));
  resize(800, 600);
  // setFixedHeight(sizeHint().height());
  // setFixedWidth(sizeHint().width());
  
}

void ImagesDialog::reload()
{
  QString page("<html>");
  QDir dir("./images/" + dir_);

  QFile file(dir.path()+"/mail.info");
  QString msg("<PRE><H3>");
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    QTextStream in(&file);
    while (!in.atEnd()) {
      QString line = in.readLine();
      msg += line + "<BR>";
      // QStringList fields = line.split(';');
      // if(fields.size() == 4) {
      // 	qint16 id    = fields.takeFirst().toInt();
      // 	qint8 type   = fields.takeFirst().toInt();
      // 	bool alert   = fields.takeFirst().toInt();
      // 	QString text = fields.takeFirst();
      // 	Event event  = { id, type, alert, text };
      // 	events_.push_back(event);
      // }
    }
    msg+="</H3></PRE>";
    label_->setText(msg);
  }

  QStringList filters;
  foreach(QByteArray format, QImageReader::supportedImageFormats())
    filters += "*." + format;

  foreach(QString file, dir.entryList(filters, QDir::Files))
    page += tr("<img src=\"%1\" width=352 height=288 />").arg(file);
  
  page += "</html>";

  textBrowser_->setSearchPaths(QStringList() << dir.path());
  textBrowser_->setHtml(page);
}

ImagesDialog::~ImagesDialog()
{
  // qDebug() << "~ImagesDialog()";
}

