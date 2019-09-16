#include "serial.h"
#include "showmaclk.h"
#include <qdebug.h>
#include <QComboBox>

static serial serialDevice;
static QString lastsend;
static qint64 rBytes = 0;

serial::serial()
{
    dialog = new showmacdialog;
    this->serialPort = new QSerialPort;
}


//寻找空闲状态串口
void serial::findFreePorts(){
    dialog = new showmacdialog;
    dialog->tCom = new QComboBox();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.size(); ++i){
        if (ports.at(i).isBusy()){
            ports.removeAt(i);
            continue;
        }
        //dialog->tCom->addItem("abc");
        //dialog->tCom->addItem(QObject::tr("None"));
        dialog->tCom->addItem(ports.at(i).portName());
        qDebug()<<"Name:"<<ports.at(i).portName();
    }
    if (!ports.size()){
        QMessageBox::warning(nullptr,"Tip",QStringLiteral("找不到空闲串口"));
        return;
    }
}


//初始化串口
bool serial::initSerialPort(){
    //dialog = new showmacdialog;
    serialDevice.serialPort->setPortName(dialog->tCom->currentText());
    if (!serialDevice.serialPort->open(QIODevice::ReadWrite)){
        QMessageBox::warning(nullptr,"Tip",QStringLiteral("串口打开失败"));
        return false;
    }

    this->serialPort->setBaudRate(dialog->tBandRate->currentText().toInt());

    if (dialog->tDataBit->currentText().toInt() == 8){
        this->serialPort->setDataBits(QSerialPort::Data8);
    }else if (dialog->tDataBit->currentText().toInt() == 7){
        this->serialPort->setDataBits(QSerialPort::Data7);
    }else if (dialog->tDataBit->currentText().toInt() == 6){
        this->serialPort->setDataBits(QSerialPort::Data6);
    }else if (dialog->tDataBit->currentText().toInt() == 5){
        this->serialPort->setDataBits(QSerialPort::Data5);
    }

    if (dialog->tStopBit->currentText().toInt() == 1){
        this->serialPort->setStopBits(QSerialPort::OneStop);
    }else if (dialog->tStopBit->currentText().toInt() == 2){
        this->serialPort->setStopBits(QSerialPort::TwoStop);
    }


    if(dialog->tCheckBit->currentText() == "NoParity"){
        this->serialPort->setParity(QSerialPort::NoParity);
    }else if (dialog->tCheckBit->currentText() == "EvenParity"){
        this->serialPort->setParity(QSerialPort::EvenParity);
    }else if (dialog->tCheckBit->currentText() == "OddParity"){
        this->serialPort->setParity(QSerialPort::OddParity);
    }

    qDebug() << "opc: PortName " << dialog->tCom->currentText();
    return true;
}


//向串口发送信息
void serial::sendMsg(const QString &msg){
    qint64 wbytes = 0;
    lastsend = msg;
    QByteArray sendData = msg.toLatin1();
    wbytes = serialDevice.serialPort->write(sendData + '\r' + '\n');

    qDebug("opc: %lld, %d\n", wbytes, sendData.length());
    dialog->serialEdit->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " -> " + msg + "\r\n");
    rBytes = wbytes;

}
//接受来自串口的信息
void serial::recvMsg(){
    QByteArray msg = serialDevice.serialPort->readAll();
    QString info = msg;
    info.remove("\r");
    info.replace("\n", " ");

    if (QString::localeAwareCompare(lastsend,info) != 0) {
        qDebug() << "lastsend" << lastsend;
        qDebug() << "info" << info;
        dialog->serialEdit->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " <- " + info + "\r\n");
    }
    //do something
    //qDebug() << "opc: " << msg.toHex().toUpper();
     //qDebug()<<"Name:"<<serialDevice.serialPort->portName();
     msg.clear();
}

