#include "mycanthread.h"



MyCanThread::MyCanThread() :
    QThread()
{

}

MyCanThread::MyCanThread(const int a,const int b,const int c) :
    QThread()
{
    this->deviceTye = a;
    this->device = b;
    this->chanel = c;
    status = UNCONNECT;
}

/**
 *函数名:设置设备通道
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MyCanThread::setDeviceInfo(const int a,const int b,const int c)
{
    this->deviceTye = a;
    this->device = b;
    this->chanel = c;
}

/**
 *函数名:接收线程
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MyCanThread::run()
{
    VCI_CAN_OBJ info[100];
    VCI_ERR_INFO errInfo;
    int bufLength=0;
    DWORD rcvLength=0;
    while(!stopRcv)
    {
        Sleep(100);
        bufLength = VCI_GetReceiveNum(this->deviceTye,this->device,this->chanel);
        if(0!=bufLength)
        {
            rcvLength = VCI_Receive(this->deviceTye,this->device,this->chanel,info,100,20);
            if(0xFFFFFFFF == rcvLength)//读取失败
            {
                VCI_ReadErrInfo(this->deviceTye,this->device,this->chanel,&errInfo);
            }
            else//解析数据
            {
                //发送给数据线程处理
                emit getCanDataSignal(info,(int)rcvLength);
            }
        }
    }
}

/***********************************************/
// z 函数名称:直接发送操函数
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
void MyCanThread::sendSlot(VCI_CAN_OBJ info)
{
    if(1 == VCI_Transmit(this->deviceTye,this->device,this->chanel,&info,1))
    {
#if CLOSE_IF
        qDebug()<<"发送成功";
#endif
        QString showInfo;
        RET_IF_NOT_EAQU(Mymethod::GetInstance()->getTransmitRetQString(CAN_SEND,info,showInfo),RET_OK);
        emit sendedInfoSignal(showInfo);
    }
}
