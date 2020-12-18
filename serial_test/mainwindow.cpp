/*
 * name     date        version     modified
 * hbg      2020-09-23  V1.0        new
 * hbg      2020-10-26  V1.1        add hex translation
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // lookup for avaiable ports
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox_serial->addItem(serial.portName());
            serial.close();
        }
    }

    // set default baudrate
    ui->comboBox_baudrate->setCurrentIndex(19);
    ui->comboBox_databit->setCurrentIndex(3);
    ui->comboBox_stop->setCurrentIndex(0);
    ui->comboBox_crc->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::read_data()
{
    QByteArray buf;
    buf = serial->readAll();
    if(!buf.isEmpty())
    {
        QString str = ui->textEdit_recv->toPlainText();
        // edit by hbg, 2020-10-26, transform to hex
        if(ui->radioButton->isChecked())
        {
            qDebug() << "hex mode \n";
            QString strall = buf.toHex();
            for(quint16 i = 0; i < strall.size(); i++)
            {
                if((i % 2 == 0) && (i != 0))
                {
                    str.append(" ");
                    str.append(strall[i]);
                }
                else
                {
                    str.append(strall[i]);
                }
            }
        }
        else
        {
            qDebug() << "decimal mode \n";
            str += tr(buf);
        }
        ui->textEdit_recv->clear();
        ui->textEdit_recv->append(str);
   }
    buf.clear();
}

bool MainWindow::set_baudrate(qint32 bd)
{
    QSerialPort::BaudRate qbd;
    switch (bd)
    {
        case 1200:
            qbd = QSerialPort::Baud1200;
            break;

        case 2400:
            qbd = QSerialPort::Baud2400;
            break;

        case 4800:
            qbd = QSerialPort::Baud4800;
            break;

        case 9600:
            qbd = QSerialPort::Baud9600;
            break;

        case 19200:
            qbd = QSerialPort::Baud19200;
            break;

        case 38400:
            qbd = QSerialPort::Baud38400;
            break;

        case 57600:
            qbd = QSerialPort::Baud57600;
            break;

        case 115200:
            qbd = QSerialPort::Baud115200;
            break;

        default:
            qbd = QSerialPort::UnknownBaud;
            break;
    }

    // now set
    bool ret = serial->setBaudRate(qbd);
    if(ret)
    {
        qDebug() << "setBaudRate " << bd << " success !\n";
    }
    else
    {
        qDebug() << "setBaudRate " << bd << " fail !\n";
    }

    return ret;
}

bool MainWindow::set_databit(quint16 db)
{
    QSerialPort::DataBits qdb;
    switch (db)
    {
        case 5:
            qdb = QSerialPort::Data5;
            break;

        case 6:
            qdb = QSerialPort::Data6;
            break;

        case 7:
            qdb = QSerialPort::Data7;
            break;

        case 8:
            qdb = QSerialPort::Data8;
            break;

        default:
            qdb = QSerialPort::UnknownDataBits;
            break;
    }

    // now set...
    bool ret = serial->setDataBits(qdb);
    if(ret)
    {
        qDebug() << "setDataBits " << db << " success !\n";
    }
    else
    {
        qDebug() << "setDataBits " << db << " fail !\n";
    }

    return ret;
}

bool MainWindow::set_stopbit(double sb)
{
    QSerialPort::StopBits qsb;
    if(sb == 1.0)
    {
        qsb = QSerialPort::OneStop;
    }
    else if(sb == 1.5)
    {
        qsb = QSerialPort::OneAndHalfStop;
    }
    else if(sb == 2.0)
    {
        qsb = QSerialPort::TwoStop;
    }
    else
    {
        qsb = QSerialPort::UnknownStopBits;
    }

    // now set...
    bool ret = serial->setStopBits(qsb);
    if(ret)
    {
        qDebug() << "setStopBits " << sb << " success !\n";
    }
    else
    {
        qDebug() << "setStopBits " << sb << " fail !\n";
    }

    return ret;
}

bool MainWindow::set_checkbit(QString cb)
{
    QSerialPort::Parity qp;
    if(cb.compare("无") == 0)
    {
        qp = QSerialPort::NoParity;
    }
    else if(cb.compare("奇校验") == 0)
    {
        qp = QSerialPort::OddParity;
    }
    else if(cb.compare("偶校验") == 0)
    {
        qp = QSerialPort::OddParity;
    }
    else
    {
        qp = QSerialPort::UnknownParity;
    }

    // now set...
    bool ret = serial->setParity(qp);
    if(ret)
    {
        qDebug() << "setParity " << qp << " success !\n";
    }
    else
    {
        qDebug() << "setParity " << qp << " fail !\n";
    }

    return ret;
}


void MainWindow::on_pushButton_open_clicked()
{
    bool ret;
    serial = new QSerialPort;

    // set serial port name
    serial->setPortName(ui->comboBox_serial->currentText());

    ret = set_baudrate(ui->comboBox_baudrate->currentText().toInt());
    ret = set_databit(ui->comboBox_databit->currentText().toInt());
    ret = set_checkbit(ui->comboBox_crc->currentText());
    ret = set_stopbit(ui->comboBox_stop->currentText().toFloat());

    // set data stream
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // open
    if(serial->open(QIODevice::ReadWrite))
        qDebug() << "Open Success !!!\n";
    else
        qDebug() << "Open Fail !!!\n";

    // close
    ui->comboBox_serial->setEnabled(false);
    ui->comboBox_baudrate->setEnabled(false);
    ui->comboBox_databit->setEnabled(false);
    ui->comboBox_stop->setEnabled(false);
    ui->comboBox_crc->setEnabled(false);

    ui->pushButton_open->setEnabled(false);
    ui->pushButton_close->setEnabled(true);

   //连接信号槽
   connect(serial, SIGNAL(readyRead()), this, SLOT(read_data()), Qt::QueuedConnection);
}

void MainWindow::on_pushButton_close_clicked()
{
    ui->pushButton_open->setEnabled(true);
    ui->pushButton_close->setEnabled(false);

    qDebug() << "close now...\n";

    if(serial != nullptr)
    {
        serial->clear();
        serial->close();
        serial->deleteLater();
    }

    // enable
    ui->comboBox_serial->setEnabled(true);
    ui->comboBox_baudrate->setEnabled(true);
    ui->comboBox_crc->setEnabled(true);
    ui->comboBox_databit->setEnabled(true);
    ui->comboBox_stop->setEnabled(true);
}

void MainWindow::on_pushButton_send_clicked()
{
    QString data = ui->lineEdit_send->text();
    QByteArray DATA;
    DATA = data.toLatin1();
    if(serial)
        serial->write(DATA);
    else
        qDebug() << "serial is not open !!\n";

    ui->lineEdit_send->clear();
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->textEdit_recv->clear();
}
