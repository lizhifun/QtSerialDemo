#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>        //提供访问串口的功能
#include <QSerialPortInfo>    //提供系统中存在的串口的信息
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_curPort(new QSerialPort(this))
{
    ui->setupUi(this);

    ui->tb_PName->clear();
    //查找所有可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->tb_PName->addItem(info.portName());//添加到界面可选
    }
    ui->tb_PName->setCurrentIndex(0);
    //初始化界面参数
    SetUIData();
    //连接信号和槽 收取信息
    QObject::connect(m_curPort, &QSerialPort::readyRead, this, &MainWindow::GetPortRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetUIData()
{
    QStringList baudList;//波特率
    QStringList parityList;//校验位
    QStringList dataBitsList;//数据位
    QStringList stopBitsList;//停止位

    baudList.clear();
    baudList<<"600"<<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200"<<"230400";
    ui->tb_PRate->clear();
    ui->tb_PRate->addItems(baudList);
    ui->tb_PRate->setCurrentIndex(4);

    parityList.clear();
    parityList<<QStringLiteral("无")<<QStringLiteral("奇")<<QStringLiteral("偶");
    ui->tb_PParity->clear();
    ui->tb_PParity->addItems(parityList);
    ui->tb_PParity->setCurrentIndex(0);

    dataBitsList.clear();
    dataBitsList<<"5"<<"6"<<"7"<<"8";
    ui->tb_PBit->clear();
    ui->tb_PBit->addItems(dataBitsList);
    ui->tb_PBit->setCurrentIndex(3);

    stopBitsList.clear();
    stopBitsList<<"1"<<"1.5"<<"2";
    ui->tb_PStop->clear();
    ui->tb_PStop->addItems(stopBitsList);
    ui->tb_PStop->setCurrentIndex(0);

}

void MainWindow::on_btn_Find_clicked()
{
    ui->tb_PName->clear();
    //查找所有可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->tb_PName->addItem(info.portName());
    }
    ui->tb_PName->setCurrentIndex(0);
}

void MainWindow::on_btn_Open_clicked()
{
    m_curPort->setPortName(ui->tb_PName->currentText());//串口名
    m_curPort->setBaudRate(ui->tb_PRate->currentText().toInt());//波特率
    m_curPort->setDataBits((QSerialPort::DataBits)ui->tb_PBit->currentText().toInt());//数据位
    switch(ui->tb_PParity->currentIndex())//奇偶校验
    {
        case 0: m_curPort->setParity(QSerialPort::NoParity); break;
        case 1: m_curPort->setParity(QSerialPort::EvenParity); break;
        case 2: m_curPort->setParity(QSerialPort::OddParity); break;
        default: break;
    }
    switch(ui->tb_PStop->currentIndex())//停止位
    {
        case 0: m_curPort->setStopBits(QSerialPort::OneStop); break;
        case 1: m_curPort->setStopBits(QSerialPort::OneAndHalfStop); break;
        case 2: m_curPort->setStopBits(QSerialPort::TwoStop); break;
        default: break;
    }
    m_curPort->setFlowControl(QSerialPort::NoFlowControl);//流控制
    if(!m_curPort->open(QIODevice::ReadWrite))//读写方式打开串口
    {
        QMessageBox::about(this,QStringLiteral("提示"),QStringLiteral("串口打开失败!"));
        return;
    }
    //参数设置失能
    ui->tb_PBit->setEnabled(false);
    ui->tb_PName->setEnabled(false);
    ui->tb_PRate->setEnabled(false);
    ui->tb_PStop->setEnabled(false);
    ui->tb_PParity->setEnabled(false);

    ui->btn_Find->setEnabled(false);
    ui->btn_Open->setEnabled(false);
    ui->btn_Close->setEnabled(true);
    //发送使能
    ui->btn_Send->setEnabled(true);
}

void MainWindow::on_btn_Close_clicked()
{
    //关闭串口
    m_curPort->close();
    //下拉菜单控件使能
    ui->tb_PBit->setEnabled(true);
    ui->tb_PName->setEnabled(true);
    ui->tb_PRate->setEnabled(true);
    ui->tb_PStop->setEnabled(true);
    ui->tb_PParity->setEnabled(true);

    ui->btn_Find->setEnabled(true);
    ui->btn_Open->setEnabled(true);
    ui->btn_Close->setEnabled(false);
    //发送按键失能
    ui->btn_Send->setEnabled(false);
}

void MainWindow::on_btn_Send_clicked()
{
    //获取界面上的数据并转换成utf8格式的字节流
    QByteArray data = ui->tb_Send->toPlainText().toUtf8();
    m_curPort->write(data);
}

void MainWindow::GetPortRead()
{
    //从接收缓冲区中读取数据
    QByteArray buffer = m_curPort->readAll();
    //从界面中读取以前收到的数据
    QString recv = QDateTime::currentDateTime().toString("HH:mm:ss.mmm")+": ";
    recv+=QString(buffer);
    //显示信息
    ui->tb_Receive->append(recv);
}

void MainWindow::on_tb_PName_currentTextChanged(const QString &arg1)
{
    if(ui->tb_PName->count() > 0)
    {
        ui->btn_Open->setEnabled(true);
    }
}
