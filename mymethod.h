#ifndef MYMETHOD_H
#define MYMETHOD_H
#include "includes.h"
#include "ControlCAN.h"
class Mymethod
{
private:
    Mymethod();
public:
    static Mymethod *GetInstance();
public:
    int getBytesFromQString(QString& str,QByteArray& ba);//十六进制字符串转数组
    int getTransmitRetQString(int way,const VCI_CAN_OBJ& obj,QString& str);//将obj转化为一行可视化的字符串
    int addSpaceInQString(QString& str);//每两个字符加一个空格

    int getPacketType(const DWORD& id);//获取报文类型
    int getSourceAdres(const DWORD& id);//获取源地址
    int getAimAdres(const DWORD& id);//获取源地址
    int getHeart(const DWORD& id);//获取心跳
};

#endif // MYMETHOD_H
