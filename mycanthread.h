#ifndef MYCANTHREAD_H
#define MYCANTHREAD_H


#include "includes.h"
#include "mymethod.h"
/*这个类主要用来接收和发送can总线数据*/
class MyCanThread : public QThread
{
    Q_OBJECT

public:
    int status=UNINIT;//状态
    volatile bool stopRcv=false;//停止接收标志位
public:
    MyCanThread();
    MyCanThread(const int,const int,const int);
    void setDeviceInfo(const int,const int,const int);
    inline void setRcvWork(bool stop){this->stopRcv = stop;}//外部通过此接口进行退出当前线程
protected:
    void run();
private:
    DWORD deviceTye=4,device=0,chanel=0;//当前设备信息
    VCI_CAN_OBJ sendObj;
private slots:
    void sendSlot(VCI_CAN_OBJ info);
signals:
    void sendedInfoSignal(const QString& info);
    void rcvedInfoSignal(const QString& info);

    void getCanDataSignal(PVCI_CAN_OBJ objs,int count);//把接收到的多帧can数据发给解析线程
private:
};
#endif // MYCANTHREAD_H
