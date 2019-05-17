void on_pushButton_3_clicked();
void on_pushButton_12_clicked();
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "includes.h"
#include "mycanthread.h"
#include "myparse.h"
#include "mymethod.h"
#include "canmanager.h"
#include "makepacket.h"
#include "mytimer.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
private:
    Ui::MainWindow *ui;
    void init();//初始化

/*CAN1对象相关变量及函数*/
private://CAN1对象
    MyParse parseCAN1;QThread parseThreadCAN1;
    MakePacket mkPacketCAN1;
    std::vector<MyTimer*> can1TimersVc;//循环生成发送can数据的信号
    MyTimer* can1SingalSendTimer;
    QThread sendThreadCAN1;
    QTimer timerCAN1;
    QString sendInfoCAN1,rcvInfoCAN1;
private slots:
    void sendedInfoCAN1Slot(const QString& info);
    void rcvedInfoCAN1Slot(const QString& info);
    void timerOutCAN1Slot();
    void addSendFramCAN1Slot(const CAN_SEND_FRAME_STRUCT& info);
private slots:
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_10_clicked();
    void on_comboBox_3_currentIndexChanged(int index);
    void on_comboBox_4_currentIndexChanged(int index);
    void on_checkBox_clicked(bool checked);

/*CAN1对象相关变量及函数*/
private://CAN2对象
    MyParse parseCAN2;QThread parseThreadCAN2;
    MakePacket mkPacketCAN2;
    std::vector<MyTimer*> can2TimersVc;
    MyTimer* can2SingalSendTimer;
    QThread sendThreadCAN2;
    QTimer timerCAN2;
    QString sendInfoCAN2,rcvInfoCAN2;
private slots:
    void sendedInfoCAN2Slot(const QString& info);
    void rcvedInfoCAN2Slot(const QString& info);
    void timerOutCAN2Slot();
    void addSendFramCAN2Slot(const CAN_SEND_FRAME_STRUCT& info);
private slots:
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_comboBox_5_currentIndexChanged(int index);

    void on_comboBox_6_currentIndexChanged(int index);
    void closeEvent(QCloseEvent *event);
    void on_pushButton_4_clicked();

private:
};

#endif // MAINWINDOW_H
