/*
 * name     date        version     modified
 * hbg      2020-10-09  V1.0        new
 * hbg      2020-10-10  V1.1        modified the paramers config
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool set_baudrate(qint32 bd);
    bool set_databit(quint16 db);
    bool set_checkbit(QString cb);
    bool set_stopbit(double sb);

private slots:

    void read_data();

    void on_pushButton_open_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_clear_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
};
#endif // MAINWINDOW_H
