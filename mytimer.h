#ifndef MYTIMER_H
#define MYTIMER_H

#include "includes.h"
#include "mymethod.h"
class MyTimer : public QThread
{
    Q_OBJECT
public:
    MyTimer(bool isSendOneTime=false);
    int doSendTimer(const VCI_CAN_OBJ& info,const int time);
    inline void stopSend(){this->stop = true;}
signals:
    void canSendSignal(VCI_CAN_OBJ info);
private://循环发送
    VCI_CAN_OBJ obj;
    void run();
    volatile bool stop = false;
    int time=0;
    void test(int a=0);
private://单次发送
    volatile bool isSendOneTime;
    std::vector<VCI_CAN_OBJ> vcCan;
    std::vector<int> msleeps;
public:
    int setSingalSendData(const std::vector<VCI_CAN_OBJ>& vcCan,std::vector<int>& msleeps);
private:
};
extern bool auto_add_id;
#endif // MYTIMER_H
