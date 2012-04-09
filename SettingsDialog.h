#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QSpinBox;
class QGroupBox;
class QDialogButtonBox;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
  SettingsDialog(QWidget *parent = 0);
  ~SettingsDialog();
    
private slots:
    void ok();

private:
    QLabel *portLabel;
    QLabel *camIP1Label;
    QLabel *camIP2Label;
    QLabel *camIP3Label;
    QLabel *camFPSLabel;

    QLineEdit *portEdit;
    QLineEdit *camIP1Edit;
    QLineEdit *camIP2Edit;
    QLineEdit *camIP3Edit;
    QSpinBox *camFPSEdit;

    QGroupBox *mainGroup;
    QGroupBox *videoGroup;
    
    QDialogButtonBox *buttonBox;
};

#endif
