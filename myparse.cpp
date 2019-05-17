#include "myparse.h"
#include "mymethod.h"
MyParse::MyParse(QObject *parent) : QObject(parent)
{

}

/**
 *函数名:获取can接收数据结构体
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MyParse::getCanDataSlot(PVCI_CAN_OBJ objs,int count)
{
    //发送给主界面显示
    QString showInfo;
    //qDebug()<<"count:"<<count;
    for(int i=0;i<count;i++)
    {
        int packetType = Mymethod::GetInstance()->getPacketType(objs[i].ID);
        int sourAddr = Mymethod::GetInstance()->getSourceAdres(objs[i].ID);
        if((packetType != filterType)&&(filterType != ALL_PACKET))//过滤报文类型
        {
            //qDebug()<<"packetType:"<<packetType;
            continue;
        }
        if((sourAddr != filterSourAddr)&&(filterSourAddr != ALL_ADDR))//过滤源地址
        {
            //qDebug()<<"sourAddr:"<<sourAddr;
            continue;
        }
        //qDebug()<<"get a packet:"<<packetType;
        PackeManager packet(packetType);
        packet.setPacket(objs[i]);
        showInfo += packet.getMeaningStr(CAN_RCV)+"\n";
    }
    if(showInfo!="")
    {
        emit showInfoSignal(showInfo.trimmed());
    }
}
