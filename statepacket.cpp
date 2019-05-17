#include "statepacket.h"
#include "ui_statepacket.h"

StatePacket::StatePacket(QWidget *parent) :
    QDialog(parent),
    PackeManager(STATE_PACKET),
    ui(new Ui::StatePacket)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
}

StatePacket::~StatePacket()
{
    delete ui;
}

/**
 *函数名:根据界面选择返回obj
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
VCI_CAN_OBJ StatePacket::getObj()
{
    memset(&obj,0,sizeof(obj));
    QByteArray ba;ba.resize(8);memset(ba.data(),0,ba.size());
    char c = 0x00;
    //采集模块类型
    if(0==ui->comboBox->currentIndex())
    {
        c = c | (0x01<<4);
    }
    else if(1==ui->comboBox->currentIndex())
    {
        c = c | (0x02<<4);
    }
    else if(2==ui->comboBox->currentIndex())
    {
        c = c | (0x03<<4);
    }
    else if(3==ui->comboBox->currentIndex())
    {
        c = c | (0x04<<4);
    }
    else if(4==ui->comboBox->currentIndex())
    {
        c = c | (0x05<<4);
    }
    else if(5==ui->comboBox->currentIndex())
    {
        c = c | (0x06<<4);
    }
    //采集模块工作状态
    if(0==ui->comboBox_2->currentIndex())
    {
        c = c | (0x01<<0);
    }
    else if(1==ui->comboBox_2->currentIndex())
    {
        c = c | (0x02<<0);
    }
    else if(2==ui->comboBox_2->currentIndex())
    {
        c = c | (0x03<<0);
    }
    else if(3==ui->comboBox_2->currentIndex())
    {
        c = c | (0x04<<0);
    }
    else if(4==ui->comboBox_2->currentIndex())
    {
        c = c | (0x05<<0);
    }
    ba[0] = c;c=0x00;

    //采集模块编号
    ba[1] = (char)ui->lineEdit->text().toUInt();

    //采集模块工作模式
    if(0==ui->comboBox_3->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(1==ui->comboBox_3->currentIndex())
    {
        c = c | (0x02<<6);
    }
    //相机1相机状态
    if(0==ui->comboBox_17->currentIndex())
    {
        c = c | (0x00<<4);
    }
    else if(1==ui->comboBox_17->currentIndex())
    {
        c = c | (0x01<<4);
    }
    else if(2==ui->comboBox_17->currentIndex())
    {
        c = c | (0x02<<4);
    }
    else if(3==ui->comboBox_17->currentIndex())
    {
        c = c | (0x03<<4);
    }
    //相机1光源状态
    if(0==ui->comboBox_18->currentIndex())
    {
        c = c | (0x00<<2);
    }
    else if(1==ui->comboBox_18->currentIndex())
    {
        c = c | (0x01<<2);
    }
    else if(2==ui->comboBox_18->currentIndex())
    {
        c = c | (0x02<<2);
    }
    else if(3==ui->comboBox_18->currentIndex())
    {
        c = c | (0x03<<2);
    }
    //相机1除尘风扇状态
    if(0==ui->comboBox_19->currentIndex())
    {
        c = c | (0x00<<0);
    }
    else if(1==ui->comboBox_19->currentIndex())
    {
        c = c | (0x01<<0);
    }
    else if(2==ui->comboBox_19->currentIndex())
    {
        c = c | (0x02<<0);
    }
    else if(3==ui->comboBox_19->currentIndex())
    {
        c = c | (0x03<<0);
    }
    ba[2] = c;c=0x00;

    //相机1散热风扇1状态
    if(0==ui->comboBox_20->currentIndex())
    {
        c = c | (0x00<<6);
    }
    else if(1==ui->comboBox_20->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(2==ui->comboBox_20->currentIndex())
    {
        c = c | (0x02<<6);
    }
    else if(3==ui->comboBox_20->currentIndex())
    {
        c = c | (0x03<<6);
    }
    //相机1散热风扇2状态
    if(0==ui->comboBox_21->currentIndex())
    {
        c = c | (0x00<<4);
    }
    else if(1==ui->comboBox_21->currentIndex())
    {
        c = c | (0x01<<4);
    }
    else if(2==ui->comboBox_21->currentIndex())
    {
        c = c | (0x02<<4);
    }
    else if(3==ui->comboBox_21->currentIndex())
    {
        c = c | (0x03<<4);
    }
    //相机2相机状态
    if(0==ui->comboBox_12->currentIndex())
    {
        c = c | (0x00<<2);
    }
    else if(1==ui->comboBox_12->currentIndex())
    {
        c = c | (0x01<<2);
    }
    else if(2==ui->comboBox_12->currentIndex())
    {
        c = c | (0x02<<2);
    }
    else if(3==ui->comboBox_12->currentIndex())
    {
        c = c | (0x03<<2);
    }
    //相机2光源状态
    if(0==ui->comboBox_13->currentIndex())
    {
        c = c | (0x00<<0);
    }
    else if(1==ui->comboBox_13->currentIndex())
    {
        c = c | (0x01<<0);
    }
    else if(2==ui->comboBox_13->currentIndex())
    {
        c = c | (0x02<<0);
    }
    else if(3==ui->comboBox_13->currentIndex())
    {
        c = c | (0x03<<0);
    }
    ba[3] = c;c=0x00;

    //相机2除尘风扇状态
    if(0==ui->comboBox_14->currentIndex())
    {
        c = c | (0x00<<6);
    }
    else if(1==ui->comboBox_14->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(2==ui->comboBox_14->currentIndex())
    {
        c = c | (0x02<<6);
    }
    else if(3==ui->comboBox_14->currentIndex())
    {
        c = c | (0x03<<6);
    }
    //相机2散热风扇1状态
    if(0==ui->comboBox_15->currentIndex())
    {
        c = c | (0x00<<4);
    }
    else if(1==ui->comboBox_15->currentIndex())
    {
        c = c | (0x01<<4);
    }
    else if(2==ui->comboBox_15->currentIndex())
    {
        c = c | (0x02<<4);
    }
    else if(3==ui->comboBox_15->currentIndex())
    {
        c = c | (0x03<<4);
    }
    //相机2散热风扇2状态
    if(0==ui->comboBox_16->currentIndex())
    {
        c = c | (0x00<<2);
    }
    else if(1==ui->comboBox_16->currentIndex())
    {
        c = c | (0x01<<2);
    }
    else if(2==ui->comboBox_16->currentIndex())
    {
        c = c | (0x02<<2);
    }
    else if(3==ui->comboBox_16->currentIndex())
    {
        c = c | (0x03<<2);
    }
    //相机3相机状态
    if(0==ui->comboBox_4->currentIndex())
    {
        c = c | (0x00<<0);
    }
    else if(1==ui->comboBox_4->currentIndex())
    {
        c = c | (0x01<<0);
    }
    else if(2==ui->comboBox_4->currentIndex())
    {
        c = c | (0x02<<0);
    }
    else if(3==ui->comboBox_4->currentIndex())
    {
        c = c | (0x03<<0);
    }
    ba[4] = c;c=0x00;

    //相机3光源状态
    if(0==ui->comboBox_5->currentIndex())
    {
        c = c | (0x00<<6);
    }
    else if(1==ui->comboBox_5->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(2==ui->comboBox_5->currentIndex())
    {
        c = c | (0x02<<6);
    }
    else if(3==ui->comboBox_5->currentIndex())
    {
        c = c | (0x03<<6);
    }
    //相机3除尘风扇状态
    if(0==ui->comboBox_6->currentIndex())
    {
        c = c | (0x00<<4);
    }
    else if(1==ui->comboBox_6->currentIndex())
    {
        c = c | (0x01<<4);
    }
    else if(2==ui->comboBox_6->currentIndex())
    {
        c = c | (0x02<<4);
    }
    else if(3==ui->comboBox_6->currentIndex())
    {
        c = c | (0x03<<4);
    }
    //相机3散热风扇1状态
    if(0==ui->comboBox_7->currentIndex())
    {
        c = c | (0x00<<2);
    }
    else if(1==ui->comboBox_7->currentIndex())
    {
        c = c | (0x01<<2);
    }
    else if(2==ui->comboBox_7->currentIndex())
    {
        c = c | (0x02<<2);
    }
    else if(3==ui->comboBox_7->currentIndex())
    {
        c = c | (0x03<<2);
    }
    //相机3散热风扇2状态
    if(0==ui->comboBox_8->currentIndex())
    {
        c = c | (0x00<<0);
    }
    else if(1==ui->comboBox_8->currentIndex())
    {
        c = c | (0x01<<0);
    }
    else if(2==ui->comboBox_8->currentIndex())
    {
        c = c | (0x02<<0);
    }
    else if(3==ui->comboBox_8->currentIndex())
    {
        c = c | (0x03<<0);
    }
    ba[5] = c;c=0x00;

    //防护门状态
    if(0==ui->comboBox_9->currentIndex())
    {
        c = c | (0x00<<6);
    }
    else if(1==ui->comboBox_9->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(2==ui->comboBox_9->currentIndex())
    {
        c = c | (0x02<<6);
    }
    else if(3==ui->comboBox_9->currentIndex())
    {
        c = c | (0x03<<6);
    }
    //水位报警
    c = c | (((char)ui->lineEdit_2->text().toInt()<<4)&0x30);
    //箱体散热风扇状态
    if(0==ui->comboBox_10->currentIndex())
    {
        c = c | (0x00<<2);
    }
    else if(1==ui->comboBox_10->currentIndex())
    {
        c = c | (0x01<<2);
    }
    else if(2==ui->comboBox_10->currentIndex())
    {
        c = c | (0x02<<2);
    }
    else if(3==ui->comboBox_10->currentIndex())
    {
        c = c | (0x03<<2);
    }
    //加热器故障状态
    if(0==ui->comboBox_11->currentIndex())
    {
        c = c | (0x00<<0);
    }
    else if(1==ui->comboBox_11->currentIndex())
    {
        c = c | (0x01<<0);
    }
    else if(2==ui->comboBox_11->currentIndex())
    {
        c = c | (0x02<<0);
    }
    else if(3==ui->comboBox_11->currentIndex())
    {
        c = c | (0x03<<0);
    }
    ba[6] = c;c=0x00;


    obj.DataLen = ba.size();
    memcpy(obj.Data,ba.data(),ba.size());


    return obj;
}
