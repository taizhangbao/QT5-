#include "showmaclk.h"
#include "ui_showmaclk.h"
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QtPrintSupport>
#endif
#endif
#include <qdebug.h>
#include <qstring.h>
#include <QTextBrowser>
#include <QTextCursor>

static QString lastsend;
static qint64 rBytes = 0;

ShowmacLk::ShowmacLk()
{
    initShowMacLk();

    findFreePorts();

    connect(dialog->openSerialCom, SIGNAL(clicked()), this, SLOT(startSerialCom()));
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    connect(dialog->refreshComBtn, SIGNAL(clicked()), this, SLOT(findFreePorts()));
    connect(dialog->sendCmd, &QPushButton::clicked, this, [=](){
        sendMsg(dialog->inputEdit->toPlainText());
    });

    connect(dialog->serialEdit, SIGNAL(cursorPositionChanged()), this, SLOT(autoScroll()));

    createActions();
    createStatusBar();
    createDockWindows();

    resize(1000,700);
    setStatusTip(tr("Ready"));
    setWindowTitle(tr("SHM_LK_TOOL"));
}

void ShowmacLk::save()
{
    QMimeDatabase mimeDatabase;
    QString fileName;

    QString s = dialog->serialEdit->toPlainText();
    if( !s.isEmpty() )
    {
         int ret = QMessageBox::information (this, tr("My Application"),
                   tr("The document has been modified.\n"
                      "Do you want to save ?"),
                   QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Save);

        if(ret ==  QMessageBox::Save)
        {
            fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Choose a file name"), ".",
                                                    mimeDatabase.mimeTypeForName("text/txt").filterString());

            if ( fileName.isEmpty() )   //user don't selected any file
            {
               return;
            }

            QFile destFile(fileName);

            if (!destFile.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, tr("Dock Widgets"),
                                     tr("Cannot write file %1:\n%2.")
                                     .arg(QDir::toNativeSeparators(fileName), destFile.errorString()));
                return;
            }
            QTextStream cacheStream(&destFile);
            cacheStream <<s;
            destFile.close();
        }
    }
    statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);
}

void ShowmacLk::print()
{
#if QT_CONFIG(printdialog)
    QTextDocument *document = dialog->serialEdit->document();
    QPrinter printer;

    QPrintDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    document->print(&printer);
    statusBar()->showMessage(tr("Ready"), 2000);
#endif
}

void ShowmacLk::about()
{
   //QMessageBox::about(this, tr("About SHM_LK_TOOL"),
   //         tr("Copyright(c) Showmac Tech Inc\ntaizhangbao@showmac.cn"));

   QMessageBox::information(this, "About SHM_LK_TOOL",
       "Copyright(c) Showmac Tech Inc \n"
       "taizhangbao@showmac.cn");
}

void ShowmacLk::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
#if 0
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newLetterAct = new QAction(newIcon, tr("&New"), this);
    newLetterAct->setShortcuts(QKeySequence::New);
    newLetterAct->setStatusTip(tr("Create a new file"));
    //connect(newLetterAct, &QAction::triggered, this, &ShowmacLk::newLetter);
    fileMenu->addAction(newLetterAct);
    fileToolBar->addAction(newLetterAct);
#endif
    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current log"));
    connect(saveAct, &QAction::triggered, this, &ShowmacLk::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(":/images/print.png"));
    QAction *printAct = new QAction(printIcon, tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current log"));
    connect(printAct, &QAction::triggered, this, &ShowmacLk::print);
    fileMenu->addAction(printAct);
    fileToolBar->addAction(printAct);

    fileMenu->addSeparator();

    QAction *quitAct = fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
    quitAct->setShortcut(tr("Ctrl+Q"));
    quitAct->setStatusTip(tr("Quit the application"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));

    const QIcon clearIcon = QIcon::fromTheme("log-clean", QIcon(":/images/clearlog.png"));
    spClearLogAct = new QAction(clearIcon,tr("&Clear Screen"),this);
    spClearLogAct->setShortcut(tr("Ctrl+L") );
    spClearLogAct->setStatusTip("Clear the LogImpl Message");
    connect(spClearLogAct,SIGNAL(triggered()),this, SLOT(slotClearLog()));
    spClearLogAct->setStatusTip(tr("Clear the log message"));
    editMenu->addAction(spClearLogAct);
    editToolBar->addAction(spClearLogAct);

    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(":/images/undo.png"));
    QAction *undoAct = new QAction(undoIcon, tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, &QAction::triggered, this, &ShowmacLk::undo);
    editMenu->addAction(undoAct);
    editToolBar->addAction(undoAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    menuBar()->addSeparator();

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &ShowmacLk::about);
    aboutAct->setShortcut(tr("Ctrl+H"));
    aboutAct->setStatusTip(tr("Show the showmac load key tool's About box"));

    //QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    //aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void ShowmacLk::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void ShowmacLk::creatGridLayout()
{
    //QVBoxLayout *groupsLayout = new QVBoxLayout;
        //textL->addWidget(keyG);
        //textL->addWidget(srcG);
        //textL->addWidget(resultG);

    dialog->serialWid = new QGroupBox(tr("COM Port Setting"));
    dialog->serialLayout = new QGridLayout();
    dialog->serialLayout->setColumnStretch(1, 6);

    dialog->serialLayout->addWidget(new QLabel("COM Port"),0,0);
    dialog->serialLayout->addWidget(dialog->tCom,0,1);

    dialog->tBandRate = new QComboBox();
    dialog->serialLayout->addWidget(new QLabel("BaudRate"),1,0);
    QStringList baudRateList;
    baudRateList <<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200"<<"230400"<<"460800"<<"921600";
    dialog->tBandRate->addItems(baudRateList);
    dialog->serialLayout->addWidget(dialog->tBandRate,1,1);

    dialog->tDataBit = new QComboBox();
    dialog->serialLayout->addWidget(new QLabel("ByteSize"),2,0);
    QStringList dataBitList;
    dataBitList <<"4"<<"5"<<"6"<<"7"<<"8";
    dialog->tDataBit->addItems(dataBitList);
    dialog->serialLayout->addWidget(dialog->tDataBit,2,1);


    dialog->tStopBit = new QComboBox();
    dialog->serialLayout->addWidget(new QLabel("Stopbits"),3,0);
    QStringList stopBitList;
    stopBitList <<"1"<<"1.5"<<"2";
    dialog->tStopBit->addItems(stopBitList);
    dialog->serialLayout->addWidget(dialog->tStopBit,3,1);


    dialog->tCheckBit = new QComboBox();
    dialog->serialLayout->addWidget(new QLabel("Parity"),4,0);
    dialog->tCheckBit->addItem(QObject::tr("None"));
    dialog->tCheckBit->addItem(QObject::tr("Odd"));
    dialog->tCheckBit->addItem(QObject::tr("Even"));
    dialog->tCheckBit->addItem(QObject::tr("Mark"));
    dialog->tCheckBit->addItem(QObject::tr("Space"));
    dialog->serialLayout->addWidget(dialog->tCheckBit,4,1);



    dialog->serialLayout->addWidget(dialog->openSerialCom,5,0);
    dialog->serialLayout->addWidget(dialog->refreshComBtn,5,1);
    dialog->serialLayout->setRowStretch(0, 0);
    dialog->serialLayout->setRowStretch(1, 0);
    dialog->serialLayout->setRowStretch(2, 0);
    dialog->serialLayout->setRowStretch(3, 0);
    dialog->serialLayout->setRowStretch(4, 0);
    dialog->serialLayout->setRowStretch(5, 0);
    dialog->serialLayout->setRowStretch(6, 8);
    dialog->serialWid->setLayout(dialog->serialLayout);
}

void ShowmacLk::creatBottomGridLayout()
{
    //dialog->serialWid = new QGroupBox(tr("COM Port Setting"));
    dialog->inputWid = new QWidget(this);
    dialog->inputLayout = new QGridLayout();
    //dialog->inputLayout->setColumnStretch(1, 1);

    dialog->inputLayout->addWidget(dialog->inputEdit,0,0);
    dialog->inputLayout->addWidget(dialog->sendCmd,0,1);

    dialog->inputWid->setLayout(dialog->inputLayout);
}
#if 0
void ShowmacLk::creatHttpGridLayout()
{
    httpsWid = new QGroupBox(tr("HTTPS Setting"));

   // dialog->serialWid->setLayout(dialog->serialLayout);
}
#endif
void ShowmacLk::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Setting"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    creatGridLayout();

    dock->setWidget(dialog->serialWid);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
    dock->setStatusTip(tr("PLS setting serial port"));

    QDockWidget *dockBottom = new QDockWidget(tr(""), this);
    dockBottom->setAllowedAreas(Qt::BottomDockWidgetArea);
    dockBottom->setFeatures(QDockWidget::NoDockWidgetFeatures);

    //dock->setWidget(httpsWid);

    QWidget* titleWidget = new QWidget(this);
    dockBottom->setTitleBarWidget( titleWidget );

    creatBottomGridLayout();
    dockBottom->setWidget(dialog->inputWid);
    addDockWidget(Qt::BottomDockWidgetArea, dockBottom);
    dockBottom->setMinimumHeight (100);
    dockBottom->setMaximumHeight (100);
    dockBottom->setStatusTip(tr("PLS input command"));
    //viewMenu->addAction(dockBottom->toggleViewAction());
}

void ShowmacLk::findFreePorts(){
    dialog->tCom->clear();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.size(); ++i){
        if (ports.at(i).isBusy()){
            ports.removeAt(i);
            continue;
        }

        dialog->tCom->addItem(ports.at(i).portName());
        qDebug()<<"Name:"<<ports.at(i).portName();
    }
    if (!ports.size()){
        QMessageBox::warning(nullptr,"Tip",QStringLiteral("找不到空闲串口"));
        return;
    }
}

//初始化串口
bool ShowmacLk::initSerialPort(){
    //dialog = new showmacdialog;
    serialPort->setPortName(dialog->tCom->currentText());
    if (!serialPort->open(QIODevice::ReadWrite)){
        QMessageBox::warning(nullptr,"Tip",QStringLiteral("Open COM Port Fail"));
        return false;
    }

    serialPort->setBaudRate(dialog->tBandRate->currentText().toInt());

    if (dialog->tDataBit->currentText().toInt() == 8){
        serialPort->setDataBits(QSerialPort::Data8);
    }else if (dialog->tDataBit->currentText().toInt() == 7){
        serialPort->setDataBits(QSerialPort::Data7);
    }else if (dialog->tDataBit->currentText().toInt() == 6){
        serialPort->setDataBits(QSerialPort::Data6);
    }else if (dialog->tDataBit->currentText().toInt() == 5){
        serialPort->setDataBits(QSerialPort::Data5);
    }

    if (dialog->tStopBit->currentText().toInt() == 1){
        serialPort->setStopBits(QSerialPort::OneStop);
    }else if (dialog->tStopBit->currentText().toInt() == 2){
        serialPort->setStopBits(QSerialPort::TwoStop);
    }


    if(dialog->tCheckBit->currentText() == "NoParity"){
        serialPort->setParity(QSerialPort::NoParity);
    }else if (dialog->tCheckBit->currentText() == "EvenParity"){
        serialPort->setParity(QSerialPort::EvenParity);
    }else if (dialog->tCheckBit->currentText() == "OddParity"){
        serialPort->setParity(QSerialPort::OddParity);
    }

    qDebug() << "opc: PortName " << dialog->tCom->currentText();
    return true;
}

//向串口发送信息
void ShowmacLk::sendMsg(const QString &msg){
    qint64 wbytes = 0;
    lastsend = msg.toUpper();

    QByteArray sendData = msg.toLatin1();
    wbytes = serialPort->write(sendData + '\r' + '\n');

    dialog->serialEdit->insertPlainText("[" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss") + "]" + msg + "\r\n");
    rBytes = wbytes;

}
//接受来自串口的信息
void ShowmacLk::recvMsg(){
    int i;
    QString atStr = "AT+";

    QByteArray msg = serialPort->readAll();
    QString info = msg.toUpper();
    info.remove("\r");

    if (QString::localeAwareCompare(lastsend,info) != 0) {
        //qDebug() << "lastsend" << lastsend;
        //qDebug() << "info" << info;
        QStringList list = info.split("\n");

        for(i = 0; i < list.count(); i++)
        {
            //qDebug()<< list[i];
            if((QString::compare(list[i], "") != 0) && (QString::compare(list[i], "AT") != 0) && (list[i].startsWith(atStr) == FALSE))
            {
                dialog->serialEdit->insertPlainText("[" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss") + "]" + list[i] + "\r\n");
            }
        }
        dialog->serialEdit->insertPlainText("\r\n");
    }

    msg.clear();
}

void ShowmacLk::initShowMacLk()
{
    serialPort = new QSerialPort;
    dialog = new showmacdialog;
    dialog->tCom = new QComboBox(this);
    dialog->openSerialCom = new QPushButton(QString("Open Port"),this);
    QFont boldFont = dialog->openSerialCom ->font();
    boldFont.setBold(true);//字体加粗
    dialog->openSerialCom->setFont(boldFont);

    dialog->refreshComBtn = new QPushButton(QString("Reresh Port"),this);
    QFont boldFont2 = dialog->refreshComBtn ->font();
    boldFont2.setBold(true);//字体加粗
    dialog->refreshComBtn->setFont(boldFont2);

    dialog->serialEdit = new QTextBrowser;
    setCentralWidget(dialog->serialEdit);
    dialog->serialEdit->setStyleSheet(QString::fromUtf8 ("background-color:black"));
    dialog->serialEdit->setCurrentFont(QFont("Arial",10,QFont::Normal,false) );
    dialog->serialEdit->setTextColor("white");
    dialog->serialEdit->setStatusTip(tr("Show log"));
    dialog->serialEdit->setFocusPolicy(Qt::NoFocus);


    dialog->sendCmd = new QPushButton(QString("Send"),this);
    dialog->sendCmd->setEnabled(false);
    QFont boldFont1 = dialog->sendCmd ->font();
    boldFont1.setBold(true);//字体加粗
    dialog->sendCmd->setFont(boldFont1);

    dialog->inputEdit = new QTextEdit(this);
    dialog->inputEdit->setFocusPolicy(Qt::StrongFocus);
    dialog->inputEdit->setFocus();
    dialog->inputEdit->installEventFilter(this);//设置完后自动调用其eventFilter函数
}

void ShowmacLk::startSerialCom()
{
    QPushButton* btn= qobject_cast<QPushButton*>(sender());
    //QString text = ((QPushButton*)sender())->text();
    QString text = btn->text();
    int a = QString::compare( text, "Open Port" );
    if(a == 0)
    {
        initSerialPort();
        dialog->sendCmd->setEnabled(true);

        dialog->openSerialCom->setText("Close Port");
    }
    else
    {
        serialPort->close();
        dialog->sendCmd->setEnabled(false);
        dialog->openSerialCom->setChecked(false);
        dialog->openSerialCom->setText("Open Port");
    }
}

void ShowmacLk::autoScroll()
{
    dialog->serialEdit->moveCursor(QTextCursor::End);
}

bool ShowmacLk::eventFilter(QObject *target, QEvent *event)
{
    if(target == dialog->inputEdit)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 sendMsg(dialog->inputEdit->toPlainText());
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}

void ShowmacLk::slotClearLog()
{
    dialog->serialEdit->clear();
    dialog->serialEdit->setCurrentFont(QFont("Helvetica",10) );

    dialog->serialEdit->viewport()->setPalette(QColor("black"));
    dialog->serialEdit->setTextColor("white");
}

void ShowmacLk::undo()
{
    QTextDocument *document = dialog->inputEdit->document();
    document->undo();
}

