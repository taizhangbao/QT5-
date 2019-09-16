#ifndef SHOWMACDIALOG_H
#define SHOWMACDIALOG_H

#include <QStackedLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QList>
#include <QMessageBox>
#include <QDateTime>
#include <QGroupBox>
#include <QTextBrowser>

class showmacdialog
{
public:
    showmacdialog();

    QTextBrowser *serialEdit;
    QTextEdit *inputEdit;
    QGroupBox *serialWid;
    QPushButton *openSerialCom;
    QGridLayout *serialLayout;
    QWidget *inputWid;
    QGridLayout *inputLayout;
    QPushButton *sendCmd;
    QVBoxLayout *groupsLayout;
    QComboBox *tCom;
    QComboBox *tDataBit;
    QComboBox *tBandRate;
    QComboBox *tStopBit;
    QComboBox *tCheckBit;
    QPushButton *refreshComBtn;
};

#endif // SHOWMACDIALOG_H
