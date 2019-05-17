#include "packemanager.h"

PackeManager::PackeManager(int packetType)
{
    this->packetType = packetType;
    addrsVc.push_back("主控箱通信板");
    addrsVc.push_back("公共信息板");
    addrsVc.push_back("线阵控制板(沉箱)");
    addrsVc.push_back("线阵控制板(侧箱)");
    addrsVc.push_back("线阵控制板(车顶)");
    addrsVc.push_back("广播");

    packeetVc.push_back("信号采集板采集报文");
    packeetVc.push_back("设备间主控箱控制报文");
    packeetVc.push_back("采集模块状态报文");
    packeetVc.push_back("轨旁控制箱状态检测报文");
    packeetVc.push_back("板卡信息报文");

}

QString PackeManager::getMeaningStr(int way)
{
    QString ret;
    if(CAN_SEND == way)//发送
    {
        QDateTime dt = QDateTime::currentDateTime();
        QString timeStr = dt.toString("yyyy.MM.dd hh:mm:ss.zzz");
        QString idStr = QString("%1").arg(obj.ID,8,16,QLatin1Char('0'));
        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,obj.DataLen);
        QString dataStr = dataBa.toHex().toUpper();
        Mymethod::GetInstance()->addSpaceInQString(dataStr);
        ret = timeStr + "    " + idStr + "    " + dataStr;
    }
    else if(CAN_RCV == way)//接收
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
        Mymethod::GetInstance()->addSpaceInQString(dataStr);
        //主控箱->广播
        QString sourcStr,aimStr;
        int sourAddr,aimAddr;
        sourAddr = Mymethod::GetInstance()->getSourceAdres(obj.ID);
        aimAddr = Mymethod::GetInstance()->getAimAdres(obj.ID);
        if(sourAddr>=1 || sourcStr<=5)
        {
            sourcStr = addrsVc[sourAddr-1];
        }
        else if(31==sourAddr)
        {
            sourcStr = addrsVc[5];
        }
        else
        {
            sourcStr = "未知地址";
        }
        if((aimAddr>=1) && (aimAddr<=5))
        {
            aimStr = addrsVc[aimAddr-1];
        }
        else if(31==aimAddr)
        {
            aimStr = addrsVc[5];
        }
        else
        {
            aimStr = "未知地址";
        }
        ret = packeetVc[packetType-1] + "    " + timeStr + "    " + idStr + "    " + dataStr + "    " + QString("%1->%2").arg(sourcStr).arg(aimStr)+ "\n" + getMeaningByType();
    }
    else
    {
        qDebug()<<"err in:"<<__FILE__<<" at:"<<__LINE__;
    }


    return ret;
}

/**
 *函数名:根据当前报文类型返回对应的解析字符串
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
QString PackeManager::getMeaningByType()
{
    QString ret;
    switch (this->packetType)
    {
    case GATHER_PACKET:
    {
        ret = getGatherMeaning();
        break;
    }
    case CONTROL_PACKET:
    {
        ret = getControlMeaning();
        break;
    }
    case STATE_PACKET:
    {
        ret = getStateMeaning();
        break;
    }
    case TESTING_PACKET:
    {
        ret = getTestMeaning();
        break;
    }
    case INFO_PACKET:
    {
        ret = getInfoMeaning();
        break;
    }
    default:
        ret = "unknown packet type";
        break;
    }
    return ret;
}

/**
 *函数名:解析采集报文
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
QString PackeManager::getGatherMeaning()
{
    QString ret;
    QByteArray ba;ba.resize(obj.DataLen);memcpy(ba.data(),obj.Data,obj.DataLen);
    //出入库
    if(1==((ba[0]>>6) & 0x03))
    {
        ret += "车辆入库"+space;
    }
    else if(2==((ba[0]>>6) & 0x03))
    {
        ret += "车辆出库"+space;
    }
    else
    {
        ret += "车辆出入库未知"+space;
    }
    //传感器故障状态
    if(0x00==((ba[0]>>3) & 0x01))
    {
        ret += "左前测速传感器:正常"+space;
    }
    else if(0x01==((ba[0]>>3) & 0x01))
    {
        ret += "左前测速传感器:故障"+space;
    }
    if(0x00==((ba[0]>>2) & 0x01))
    {
        ret += "左后测速传感器:正常"+space;
    }
    else if(0x01==((ba[0]>>2) & 0x01))
    {
        ret += "左后测速传感器:故障"+space;
    }
    if(0x00==((ba[0]>>1) & 0x01))
    {
        ret += "右前测速传感器:正常"+space;
    }
    else if(0x01==((ba[0]>>1) & 0x01))
    {
        ret += "右前测速传感器:故障"+space;
    }  if(0x00==((ba[0]>>0) & 0x01))
    {
        ret += "右后测速传感器:正常"+space;
    }
    else if(0x01==((ba[0]>>0) & 0x01))
    {
        ret += "右后测速传感器:故障"+space;
    }
    //车速值
    ret += "车速:"+QString::number(((uchar)ba[1]<<8) + ((uchar)ba[2]))+space;
    //系统时间
    ret += "系统时间:"+QString::number(((uchar)ba[3]<<24) + ((uchar)ba[4]<<16) + ((uchar)ba[5]<<8) +( (uchar)ba[6]));
    return ret;
}

/**
 *函数名:解析控制报文
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
QString PackeManager::getControlMeaning()
{
    QString ret;
    QByteArray ba;ba.resize(obj.DataLen);memcpy(ba.data(),obj.Data,obj.DataLen);
    //运行维护
    if(1==((ba[0]>>6) & 0x03))
    {
        ret += "运行指令"+space;
    }
    else if(2==((ba[0]>>6) & 0x03))
    {
        ret += "维护指令"+space;
    }
    else
    {
        ret += "运行/维护指令未知"+space;
    }
    //本地远程
    if(1==((ba[0]>>4) & 0x03))
    {
        ret += "本地指令"+space;
    }
    else if(2==((ba[0]>>4) & 0x03))
    {
        ret += "远程指令"+space;
    }
    else
    {
        ret += "本地/远程指令未知"+space;
    }
    //开关门
    if(1==((ba[0]>>2) & 0x03))
    {
        ret += "开门指令"+space;
    }
    else if(2==((ba[0]>>2) & 0x03))
    {
        ret += "关门指令"+space;
    }
    else
    {
        ret += "开/关门指令未知"+space;
    }
    //除尘开关
    if(1==((ba[0]>>0) & 0x03))
    {
        ret += "除尘开"+space;
    }
    else if(2==((ba[0]>>0) & 0x03))
    {
        ret += "除尘关"+space;
    }
    else
    {
        ret += "除尘开/关指令未知"+space;
    }

    //散热开关
    if(1==((ba[1]>>6) & 0x03))
    {
        ret += "散热开指令"+space;
    }
    else if(2==((ba[1]>>6) & 0x03))
    {
        ret += "散热关指令"+space;
    }
    else
    {
        ret += "散热开/关指令指令未知"+space;
    }
    //加热开关
    if(1==((ba[1]>>4) & 0x03))
    {
        ret += "加热开指令"+space;
    }
    else if(2==((ba[1]>>4) & 0x03))
    {
        ret += "加热关指令"+space;
    }
    else
    {
        ret += "加热开/关指令未知"+space;
    }
    //线阵开关
    if(1==((ba[1]>>2) & 0x03))
    {
        ret += "线阵开指令"+space;
    }
    else if(2==((ba[1]>>2) & 0x03))
    {
        ret += "关门指令"+space;
    }
    else
    {
        ret += "线阵开/关指令未知"+space;
    }
    //画阵开关指令
    if(1==((ba[1]>>0) & 0x03))
    {
        ret += "画阵开指令"+space;
    }
    else if(2==((ba[1]>>0) & 0x03))
    {
        ret += "画阵关指令"+space;
    }
    else
    {
        ret += "画阵开/关指令未知"+space;
    }
    //模拟测试触发指令
    if(1==((ba[2]>>6) & 0x03))
    {
        ret += "模拟测试触发指令"+space;
    }
    else if(2==((ba[2]>>6) & 0x03))
    {
        ret += "模拟测试不触发指令"+space;
    }
    else
    {
        ret += "模拟测试触发指令未知"+space;
    }
    //系统工作状态
    ret += "系统工作状态:";
    if(0 == (uchar)ba[3])
    {
        ret += "系统就绪"+space;
    }
    else if(1 == (uchar)ba[3])
    {
        ret += "车辆进入状态"+space;
    }
    else if(2 == (uchar)ba[3])
    {
        ret += "维护状态"+space;
    }
    else if(3 == (uchar)ba[3])
    {
        ret += "模拟运行状态"+space;
    }
    else if(4 == (uchar)ba[3])
    {
        ret += "自检状态"+space;
    }
    else
    {
        ret += "未知"+space;
    }
    return ret;
}

/**
 *函数名:解析状态报文
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
QString PackeManager::getStateMeaning()
{
    QString ret;
    QByteArray ba;ba.resize(obj.DataLen);memcpy(ba.data(),obj.Data,obj.DataLen);
    //常量:采集模块类型
    QString gatherType[6]={"车底轨中采集箱2","车底轨中采集箱1","车底轨外采集箱","走行部侧面采集箱","车体侧面采集箱","车顶采集模块"};
    //常量:采集模块工作状态
    QString gatherWorkState[5]={"维护状态","休眠状态","激活状态","拍摄状态","错误状态"};
    //常量:状态
    QString state[4]={"模块不存在","部件打开","部件关闭","部件故障"};

    //采集模块类型
    int index = 0;
    index = (ba[0]>>4)&0x0F;
    if(index>=1 && index<=6)
    {
        ret += "模块类型:"+gatherType[index-1]+space;
    }
    else
    {
        ret += "模块类型:未知"+space;
    }
    //采集模块工作状态
    index = (ba[0]>>0)&0x0F;
    if(index>=1 && index<=5)
    {
        ret += "工作状态:"+gatherWorkState[index-1]+space;
    }
    else
    {
        ret += "工作状态:未知"+space;
    }
    //采集模块编号
    ret += "采集模块编号:"+QString::number((uchar)ba[1])+space;

    //采集模块工作模式
    if(1==((ba[2]>>6) & 0x03))
    {
        ret += "模块工作模式:运行"+nextLine;
    }
    else if(2==((ba[2]>>6) & 0x03))
    {
        ret += "模块工作模式:维护"+nextLine;
    }
    //相机1状态
    index = ba[2]>>4 & 0x03;
    ret += "相机1状态:"+state[index]+space;
    //相机1光源状态
    index = ba[2]>>2 & 0x03;
    ret += "相机1光源状态:"+state[index]+space;
    //相机1除尘风扇状态
    index = ba[2]>>0 & 0x03;
    ret += "相机1除尘风扇状态:"+state[index]+space;

    //相机1散热风扇1状态
    index = ba[3]>>6 & 0x03;
    ret += "相机1散热风扇1状态:"+state[index]+space;
    //相机1散热风扇2状态
    index = ba[3]>>4 & 0x03;
    ret += "相机1散热风扇2状态:"+state[index]+nextLine;
    //相机2状态
    index = ba[3]>>2 & 0x03;
    ret += "相机2状态:"+state[index]+space;
    //相机2光源状态
    index = ba[3]>>0 & 0x03;
    ret += "相机2光源状态:"+state[index]+space;

    //相机2除尘风扇状态
    index = ba[4]>>6 & 0x03;
    ret += "相机2除尘风扇状态:"+state[index]+space;
    //相机2散热风扇1状态
    index = ba[4]>>4 & 0x03;
    ret += "相机2散热风扇1状态:"+state[index]+space;
    //相机2散热风扇2状态
    index = ba[4]>>2 & 0x03;
    ret += "相机2散热风扇2状态:"+state[index]+nextLine;
    //相机3状态
    index = ba[4]>>0 & 0x03;
    ret += "相机3状态:"+state[index]+space;

    //相机3光源状态
    index = ba[5]>>6 & 0x03;
    ret += "相机3光源状态:"+state[index]+space;
    //相机3除尘风扇状态
    index = ba[5]>>4 & 0x03;
    ret += "相机3除尘风扇状态:"+state[index]+space;
    //相机3散热风扇1状态
    index = ba[5]>>2 & 0x03;
    ret += "相机3散热风扇1状态:"+state[index]+space;
    //相机3散热风扇2状态
    index = ba[5]>>0 & 0x03;
    ret += "相机3散热风扇2状态:"+state[index]+nextLine;

    //防护门状态
    index = ba[6]>>6 & 0x03;
    ret += "防护门状态:"+state[index]+space;
    //水位报警
    index = ba[6]>>4 & 0x03;
    ret += "水位报警:"+QString::number((ba[6]>>4 & 0x03))+space;
    //箱体散热风扇状态
    index = ba[6]>>2 & 0x03;
    ret += "箱体散热风扇状态:"+state[index]+space;
    //加热器故障状态
    index = ba[6]>>0 & 0x03;
    ret += "加热器故障状态:"+state[index];
    return ret;
}

/**
 *函数名:解析监测报文
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
QString PackeManager::getTestMeaning()
{
    QString ret;
    QByteArray ba;ba.resize(obj.DataLen);memcpy(ba.data(),obj.Data,obj.DataLen);
    //轨旁控制箱散热风扇开关
    if(1==((ba[0]>>6) & 0x03))
    {
        ret += "轨旁散热风扇:开"+space;
    }
    else if(2==((ba[0]>>6) & 0x03))
    {
        ret += "轨旁散热风扇:关"+space;
    }
    else
    {
        ret += "轨旁散热风扇:未知"+space;
    }
    //温度传感器故障状态
    if(1==((ba[0]>>4) & 0x03))
    {
        ret += "温度传感器故障状态:正常"+space;
    }
    else if(2==((ba[0]>>4) & 0x03))
    {
        ret += "温度传感器故障状态:故障"+space;
    }
    else
    {
        ret += "温度传感器故障状态:未知"+space;
    }
    //DC12V电源故障状态
    if(1==((ba[0]>>2) & 0x03))
    {
        ret += "DC12V电源故障状态:正常"+space;
    }
    else if(2==((ba[0]>>2) & 0x03))
    {
        ret += "DC12V电源故障状态:故障"+space;
    }
    else
    {
        ret += "DC12V电源故障状态:未知"+space;
    }
    //DC24V电源故障状态
    if(1==((ba[0]>>0) & 0x03))
    {
        ret += "DC24V电源故障状态:正常"+space;
    }
    else if(2==((ba[0]>>0) & 0x03))
    {
        ret += "DC24V电源故障状态:故障"+space;
    }
    else
    {
        ret += "DC24V电源故障状态:未知"+space;
    }

    //环境温度
    ret += "环境温度:"+QString::number(ba[1])+space;

    //DC5v电源电压
    ret += "DC5v电源电压:"+QString::number(((uchar)ba[2]<<8) + ((uchar)ba[3]));
    return ret;
}

/**
 *函数名:解析信息报文
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
QString PackeManager::getInfoMeaning()
{
    QString ret;
    QByteArray ba;ba.resize(obj.DataLen);memcpy(ba.data(),obj.Data,obj.DataLen);
    //生命信号
    ret += "生命信号:"+QString::number((uchar)ba[0])+space;
    //硬件主版本号
    ret += "硬件主版本号:"+QString::number((uchar)ba[1])+space;
    //硬件次版本号
    ret += "硬件次版本号:"+QString::number((uchar)ba[2])+space;
    //软件主版本号
    ret += "软件主版本号:"+QString::number((uchar)ba[3])+space;
    //软件次版本号
    ret += "软件次版本号:"+QString::number((uchar)ba[4])+space;
    //CAN1故障状态
    if(1==((ba[5]>>6) & 0x03))
    {
        ret += "CAN1故障状态:正常"+space;
    }
    else if(2==((ba[5]>>6) & 0x03))
    {
        ret += "CAN1故障状态:故障"+space;
    }
    else
    {
        ret += "CAN1故障状态:未知"+space;
    }
    //CAN2故障状态
    if(1==((ba[5]>>4) & 0x03))
    {
        ret += "CAN2故障状态:正常"+space;
    }
    else if(2==((ba[5]>>4) & 0x03))
    {
        ret += "CAN2故障状态:故障"+space;
    }
    else
    {
        ret += "CAN2故障状态:未知"+space;
    }
    return ret;
}
