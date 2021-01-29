#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SetUIData();

private slots:
    void on_btn_Find_clicked();
    void on_btn_Open_clicked();
    void on_btn_Close_clicked();
    void on_btn_Send_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *m_curPort;

private slots:
    void GetPortRead();
    void on_tb_PName_currentTextChanged(const QString &arg1);
};
#endif // MAINWINDOW_H
