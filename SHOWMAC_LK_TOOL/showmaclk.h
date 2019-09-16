#ifndef SHOWMACLK_H
#define SHOWMACLK_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
#include "showmacdialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE


class ShowmacLk : public QMainWindow
{
    Q_OBJECT

public:
    //QTextEdit *serialEdit;

    QSerialPort *serialPort;
    ShowmacLk();
protected:
    bool eventFilter(QObject *target, QEvent *event);
private slots:
    void save();
    void print();
    void recvMsg();
    void about();
    void startSerialCom();
    void sendMsg(const QString &msg);
    void autoScroll();
    void slotClearLog();
    void undo();
    void findFreePorts();
private:
    QAction *spClearLogAct;

    void createActions();
    void createStatusBar();
    void createDockWindows();
    void creatGridLayout();
    void creatBottomGridLayout();
    void creatHttpGridLayout();

    bool initSerialPort();
    void initShowMacLk();

    showmacdialog *dialog;
    QMenu *viewMenu;
};

#endif // SHOWMACLK_H
