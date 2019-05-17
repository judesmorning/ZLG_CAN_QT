#include "controlpacket.h"
#include "ui_controlpacket.h"

ControlPacket::ControlPacket(QWidget *parent) :
    QDialog(parent),
    PackeManager(CONTROL_PACKET),
    ui(new Ui::ControlPacket)
{
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->setupUi(this);
}

ControlPacket::~ControlPacket()
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
VCI_CAN_OBJ ControlPacket::getObj()
{
    memset(&obj,0,sizeof(obj));
    QByteArray ba;ba.resize(8);memset(ba.data(),0,ba.size());
    char c = 0x00;
    //除尘开关
    if(0==ui->comboBox->currentIndex())
    {
        c = c | (0x01<<0);
    }
    else if(1==ui->comboBox->currentIndex())
    {
        c = c | (0x02<<0);
    }
    //门开关
    if(0==ui->comboBox_2->currentIndex())
    {
        c = c | (0x01<<2);
    }
    else if(1==ui->comboBox_2->currentIndex())
    {
        c = c | (0x02<<2);
    }
    //本地远程
    if(0==ui->comboBox_3->currentIndex())
    {
        c = c | (0x01<<4);
    }
    else if(1==ui->comboBox_3->currentIndex())
    {
        c = c | (0x02<<4);
    }
    //运行维护
    if(0==ui->comboBox_4->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(1==ui->comboBox_4->currentIndex())
    {
        c = c | (0x02<<6);
    }
    ba[0] = c;c=0x00;

    //面阵开关
    if(0==ui->comboBox_5->currentIndex())
    {
        c = c | (0x01<<0);
    }
    else if(1==ui->comboBox_5->currentIndex())
    {
        c = c | (0x02<<0);
    }
    //线阵开关
    if(0==ui->comboBox_6->currentIndex())
    {
        c = c | (0x01<<2);
    }
    else if(1==ui->comboBox_6->currentIndex())
    {
        c = c | (0x02<<2);
    }
    //加热开关
    if(0==ui->comboBox_7->currentIndex())
    {
        c = c | (0x01<<4);
    }
    else if(1==ui->comboBox_7->currentIndex())
    {
        c = c | (0x02<<4);
    }
    //散热开关
    if(0==ui->comboBox_8->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(1==ui->comboBox_8->currentIndex())
    {
        c = c | (0x02<<6);
    }
    ba[1] = c;c=0x00;

    //模拟测试触发
    if(0==ui->comboBox_9->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(1==ui->comboBox_9->currentIndex())
    {
        c = c | (0x02<<6);
    }
    ba[2] = c;c=0x00;

    //系统工作状态
    c = (char)ui->comboBox_10->currentIndex();
    ba[3] = c;c=0x00;
    obj.DataLen = ba.size();
    memcpy(obj.Data,ba.data(),ba.size());

    return obj;
}
