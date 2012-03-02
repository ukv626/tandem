#include <QtGui>

#include "CameraDialog.h"
#include "FlexWatchCam.h"

CameraDialog::CameraDialog(const QString &ip,
			   int postNum,
			   int camNum1, int camNum2,
			   QWidget *parent)
  : QDialog(parent) /*, file_("./ukv.tmp") */
{
  flexWatchCam1_ = new FlexWatchCam(ip, camNum1);
  flexWatchCam2_ = new FlexWatchCam(ip, camNum2);

  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(flexWatchCam1_);
  layout->addWidget(flexWatchCam2_);
  setLayout(layout);

  setWindowTitle(trUtf8("Видеонаблюдение: Пост %1").arg(postNum));
  setFixedWidth(sizeHint().width());
  setFixedHeight(sizeHint().height());
}





