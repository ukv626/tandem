#include <QtGui>

#include "CameraDialog.h"
#include "ImageWidget.h"


CameraDialog::CameraDialog(QWidget *parent)
    : QDialog(parent)
{
  ipComboBox_ = new QComboBox;
  ipComboBox_->addItems(QStringList() << tr("80.246.243.35") << tr("192.168.1.45"));
  ipComboBox_->setEditable(true);
  ipLabel_ = new QLabel(trUtf8("Видеорегистратор"));
  ipLabel_->setBuddy(ipComboBox_);

  cameraComboBox_ = new QComboBox;
  cameraComboBox_->addItems(QStringList() << trUtf8("Камера 1")
			    << trUtf8("Камера 2")
			    << trUtf8("Камера 3")
			    << trUtf8("Камера 4"));
  cameraLabel_ = new QLabel(trUtf8("Камера"));
  cameraLabel_->setBuddy(cameraComboBox_);
  //label->setPixmap(QPixmap("image2.jpg"));
  
  fpsSpinBox_ = new QSpinBox;
  fpsSpinBox_->setRange(1, 100);
  fpsSpinBox_->setSingleStep(1);
  fpsSpinBox_->setValue(30);

  fpsLabel_ = new QLabel(trUtf8("Кадров/сек"));
  fpsLabel_->setBuddy(fpsSpinBox_);

  imageWidget_ = new ImageWidget;

  // startButton = new QPushButton(tr("start"));
  // startButton->setDefault(true);
  // stopButton = new QPushButton(tr("stop"));
  // closeButton = new QPushButton(tr("close"));

  
  buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok
				   | QDialogButtonBox::Cancel);
  // buttonBox->addButton(startButton, QDialogButtonBox::AcceptRole);
  // buttonBox->addButton(stopButton, QDialogButtonBox::ActionRole);
  // buttonBox->addButton(closeButton,QDialogButtonBox::RejectRole);
  
  connect(buttonBox_, SIGNAL(accepted()), this, SLOT(start()));
  connect(buttonBox_, SIGNAL(rejected()), this, SLOT(stop()));


  QGridLayout *topLayout = new QGridLayout;
  topLayout->addWidget(ipLabel_, 0, 0);
  topLayout->addWidget(ipComboBox_, 0, 1);
  topLayout->addWidget(cameraLabel_, 1, 0);
  topLayout->addWidget(cameraComboBox_, 1, 1);
  topLayout->addWidget(fpsLabel_, 2, 0);
  topLayout->addWidget(fpsSpinBox_, 2, 1);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(topLayout);
  layout->addWidget(imageWidget_);
  layout->addWidget(buttonBox_);
  setLayout(layout);

  setWindowTitle(tr("Camera"));
  //setFixedWidth(sizeHint().width());

  connect(&http_, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
  connect(&http_, SIGNAL(readyRead(const QHttpResponseHeader &)),
	  this, SLOT(httpReadyRead(const QHttpResponseHeader &)));

  
  boundary_.append(QString("--myboundary"));
}

void CameraDialog::start()
{
  ipComboBox_->setEnabled(false);
  cameraComboBox_->setEnabled(false);
  fpsSpinBox_->setEnabled(false);
  getStream();
}


bool CameraDialog::getStream()
{
  stopFlag_ = false;

  http_.setHost(ipComboBox_->currentText());
  //http.get(QString("/cgi-bin/fwsysget.cgi?FwModId=0&FwCgiVer=0x0001\r\n\r\n"));
  http_.get(QString("/cgi-bin/fwstream.cgi?FwModId=0&AppKey=0x0450f000&PortId=%1"
		    "&PauseTime=%2&FwCgiVer=0x0001\r\n\r\n").arg(cameraComboBox_->currentIndex()).arg(100 / fpsSpinBox_->value()));

  http_.close();
  return true;
}

void CameraDialog::httpDone(bool error)
{
  if(error)
    qDebug() << "Error: " << qPrintable(http_.errorString()) << "\n";
  else
    qDebug() << "Finished.\n";

  emit done();
}

void CameraDialog::httpReadyRead(const QHttpResponseHeader & resp)
{
  //Q_UNUSED(resp);
  
  if(resp.statusCode() != 200)
    qDebug() << "Error: StatusCode = " << resp.statusCode() << "\n";
  else {
    QByteArray array = http_.readAll();

     static const char new_lines_data[] = { 0x0D, 0x0A, 0x0D, 0x0A };
     QByteArray new_lines = QByteArray::fromRawData(new_lines_data, sizeof(new_lines_data));

    int boundary_pos = array.indexOf(boundary_);
    if(boundary_pos >= 0) {
      // write2File(QString("boundary.raw"), array);
      
      int new_lines_pos = array.indexOf(new_lines);
      
      if(boundary_pos>0)
	image_.append(array.left(boundary_pos));

      QPixmap pixmap;
      pixmap.loadFromData(image_);
      //label_->setPixmap(pixmap);

      imageWidget_->setImage(pixmap);
      image_.truncate(0);
      
      //std::cerr << "boundary found at " << boundary_pos << " " << new_lines_pos << " " << array.size() << "!!\n";
      ++i_;

      if(new_lines_pos+5 < array.size())
	image_.append(array.mid(new_lines_pos+4));
    }
    else
      image_.append(array);
  }

  if(stopFlag_)
    http_.abort();
}

void CameraDialog::stop()
{
  stopFlag_ = true;
  ipComboBox_->setEnabled(true);
  cameraComboBox_->setEnabled(true);
  fpsSpinBox_->setEnabled(true);
}
