#ifndef SERIAL_H
#define SERIAL_H
#include "showmaclk.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include "showmacdialog.h"

class serial// : public ShowmacLk
{
public:
    serial();
    QSerialPort *serialPort;
    void findFreePorts();
    bool initSerialPort();
    void sendMsg(const QString &msg);

    showmacdialog *dialog;
protected:


public slots:
    void recvMsg();

private:
    //ShowmacLk *showmacSerial;
    //showmacdialog *dialog;
};

#endif // SERIAL_H
