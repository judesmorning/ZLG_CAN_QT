#include "mymethod.h"

Mymethod::Mymethod()
{

}

/**
 *函数名:返回单列对象
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
Mymethod *Mymethod::GetInstance()
{
    static Mymethod gp;
    return &gp;
}

/**
 *函数名:NULL
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
int Mymethod::getBytesFromQString(QString &str, QByteArray &ba)
{
    int ret = RET_OK;
    str.remove(QRegExp("\\s"));//去掉空格
    if(0 != str.size()%2)//如果不是偶数个字符则加上0
    {
        str = "0"+str;
    }
    ba.resize(str.size()/2);
    QString tmpStr;
    int j=0;
    for(int i=0;i<str.size();i++)
    {
        tmpStr = str[i++];
        tmpStr += str[i];
        ba[j++] = tmpStr.toUInt(nullptr,Z_HEX);
    }
    return ret;
}

/**
 *函数名:返回发送或者接收到的obj结构体字符串
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
int Mymethod::getTransmitRetQString(int way, const VCI_CAN_OBJ &obj, QString &str)
{
    int ret = RET_OK;
    str.clear();
    if(CAN_SEND == way)
    {
        QDateTime dt = QDateTime::currentDateTime();
        QString timeStr = dt.toString("yyyy.MM.dd hh:mm:ss.zzz");
        QString idStr = QString("%1").arg(obj.ID,8,16,QLatin1Char('0'));
        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,obj.DataLen);
        QString dataStr = dataBa.toHex().toUpper();
        addSpaceInQString(dataStr);
        str = timeStr + "    " + idStr + "    " + dataStr;
    }
    else if(CAN_RCV == way)
    {
        QDateTime dt = QDateTime::currentDateTime();
        QString timeStr = dt.toString("yyyy.MM.dd ");
        int hour = obj.TimeStamp/36000000;
        int minute = (obj.TimeStamp-hour*36000000)/600000;
        int second = (obj.TimeStamp - hour*36000000 - minute*600000)/10000;
        int ms = (obj.TimeStamp - hour*36000000 - minute*600000 - second*10000)/10;
        QString canTimeStr = QString("%1:").arg(hour,2,10,QChar('0'));
        canTimeStr += QString("%1:").arg(minute,2,10,QChar('0'));
        canTimeStr += QString("%1.").arg(second,2,10,QChar('0'));
        canTimeStr += QString("%1").arg(ms,3,10,QChar('0'));
        timeStr += canTimeStr;
        QString idStr = QString("%1").arg(obj.ID,8,16,QLatin1Char('0'));
        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,obj.DataLen);
        QString dataStr = dataBa.toHex().toUpper();
        addSpaceInQString(dataStr);
        str = timeStr + "    " + idStr + "    " + dataStr;
    }
    else
    {
        ret = RET_ERR;
    }

    return ret;
}

/**
 *函数名:每两个字符加一个空格
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
int Mymethod::addSpaceInQString(QString &str)
{
    int ret = RET_OK;
    QString retStr;
    for(int i=0;i<str.size();i++)
    {
        retStr += str.mid(i,2);
        retStr += " ";
        i++;
    }
    str = retStr;
    return ret;
}

/**
 *函数名:获取报文种类
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
int Mymethod::getPacketType(const DWORD &id)
{
    int ret = 0;
    ret = (id>>24)&0x1F;


    return ret;
}

/***********************************************/
// z 函数名称:根据can id获取源地址
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
int Mymethod::getSourceAdres(const DWORD &id)
{
    int ret = 0;
    ret = (id>>8)&0x1F;
    return ret;

}

/***********************************************/
// z 函数名称:根据canid获取目标地址
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
int Mymethod::getAimAdres(const DWORD &id)
{
    int ret = 0;
    ret = (id>>16)&0x1F;
    return ret;
}

/**
 *函数名:根据canid获取心跳
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
int Mymethod::getHeart(const DWORD &id)
{
    int ret = 0;
    ret = (id>>0)&0xFF;
    return ret;
}
