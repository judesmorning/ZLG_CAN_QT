#ifndef MYPARSE_H
#define MYPARSE_H

#include <QObject>
#include "ControlCAN.h"
#include "statepacket.h"
/*这个类主要用来处理数据*/
class MyParse : public QObject
{
    Q_OBJECT
public:
    explicit MyParse(QObject *parent = 0);
    void setFilterType(const int a){filterType = a;}
    void setFilterSourAddr(const int a){filterSourAddr = a;}
signals:
    void showInfoSignal(QString info);//接收数据显示的信号
public slots:
    void getCanDataSlot(PVCI_CAN_OBJ objs,int count);
private:
    int filterType=GATHER_PACKET,filterSourAddr=CPU_ADDR;//过滤类型
};

#endif // MYPARSE_H
