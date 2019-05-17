#ifndef INCLUDES_H
#define INCLUDES_H

/*包含头文件*/
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QMetaType>
#include <QMessageBox>
#include <QRegExp>
#include <QPalette>
#include <QDateTime>
#include "ControlCAN.h"
#include <memory>
//通用定义

/*返回宏*/
#define PRINTF_LOCATION() qDebug()<< "ret in "<< __FILE__ << " at " <<__LINE__

#define RET_VALUE_IF_NOT_EAQU(a,b,c)  \
    do {  \
    if(a!=b) \
{        \
    PRINTF_LOCATION();\
    return c; \
}        \
    } while (false)

#define RET_VALUE_IF_EAQU(a,b,c)  \
    do {  \
    if(a==b) \
{        \
    PRINTF_LOCATION();\
    return c; \
}        \
    } while (false)

#define RET_IF_NOT_EAQU(a,b)  \
    do {  \
    if(a!=b) \
{        \
    PRINTF_LOCATION();\
    return; \
}        \
    } while (false)

#define RET_IF_EAQU(a,b)  \
    do {  \
    if(a==b) \
{        \
    PRINTF_LOCATION();\
    return; \
}        \
    } while (false)

/*进制*/
#define Z_OCTAL 8
#define Z_DECIMAL 10
#define Z_HEX 16


/*返回值枚举类型*/
enum RET_VALUE{
    RET_ERR = 0,
    RET_OK
};

/*调试开关*/
#define OPEN_IF 1
#define CLOSE_IF 0

//项目相关的定义

/*can状态*/
enum CAN_STATUS{
    UNINIT = 0,//未初始化
    UNCONNECT,//未连接
    OPENDED,//打开
    INITED,//初始化
    STARTED//就绪
};

/*can发送类型*/
enum CAN_SEND_TYPE{
    CAN_SEND_NORMAL = 0,//正常
    CAN_SEND_SIGNAL,//单次
    CAN_SEND_SELF,//自发自收
    CAN_SEND_SELF_SIGNAL//单次自发自收
};

/*can数据类型*/
enum CAN_DATA_TYPE{
    CAN_DATA_INFO=0,//数据帧
    CAN_DATA_REMOTE//远程帧
};

/*是否扩展帧*/
enum CAN_EXTERN_TYPE{
    CAN_FRAM_STANDARD=0,//标准帧
    CAN_FRAM_EXTERN//扩展帧
};

/*传输类型*/
enum CAN_TRANSMIT_TYPE{
    CAN_SEND=0,//发送
    CAN_RCV//接收
};

/*发送帧的结构体*/
struct CAN_SEND_FRAME_STRUCT
{
    CAN_SEND_FRAME_STRUCT()
    {
        packetTyepStr = "";
        idStr = "";
        dataStr = "";
        sourceStr = "";
        aimStr = "";
        timeStr = "";
        meaningStr = "";
        isSendToSelf = false;
        sendCount = 0;
    }
    QString packetTyepStr;
    QString idStr;
    QString dataStr;
    QString sourceStr;
    QString aimStr;
    QString timeStr;
    QString meaningStr;
    bool isSendToSelf;
    int sendCount;
};

/*自发类型*/
#define SEND_SELF "是"
#define SEND_NORMAL "否"


/*报文类型*/
enum PACKET_TYPE{
    GATHER_PACKET=1,//信号采集板采集报文
    CONTROL_PACKET,//设备间主控箱控制报文
    STATE_PACKET,//采集模块状态报文
    TESTING_PACKET,//轨旁控制箱状态检测报文
    INFO_PACKET,//板卡信息报文
    ALL_PACKET//所有的报文
};

/*地址定义*/
enum PACKET_ADDRES{
    CPU_ADDR=1,//主控板地址
    GATHRE_ADDR,//采集板地址
    BOTTOM_ADDR,//沉箱控制板地址
    SIDE_ADDR,//侧箱控制板地址
    TOP_ADDR,//车顶控制板地址
    BROADCAST_ADDR,//广播地址
    ALL_ADDR//所有地址
};


#endif // INCLUDES_H
