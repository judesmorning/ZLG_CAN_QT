#include "testpacket.h"
#include "ui_testpacket.h"

TestPacket::TestPacket(QWidget *parent) :
    QDialog(parent),
    PackeManager(TESTING_PACKET),
    ui(new Ui::TestPacket)
{
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->setupUi(this);
}

TestPacket::~TestPacket()
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
VCI_CAN_OBJ TestPacket::getObj()
{
    memset(&obj,0,sizeof(obj));
    QByteArray ba;ba.resize(8);memset(ba.data(),0,ba.size());
    char c = 0x00;
    //DC24v电源故障
    if(0==ui->comboBox->currentIndex())
    {
        c = c | (0x01<<0);
    }
    else if(1==ui->comboBox->currentIndex())
    {
        c = c | (0x02<<0);
    }
    //DC12v电源故障
    if(0==ui->comboBox_2->currentIndex())
    {
        c = c | (0x01<<2);
    }
    else if(1==ui->comboBox_2->currentIndex())
    {
        c = c | (0x02<<2);
    }
    //温度传感器故障
    if(0==ui->comboBox_3->currentIndex())
    {
        c = c | (0x01<<4);
    }
    else if(1==ui->comboBox_3->currentIndex())
    {
        c = c | (0x02<<4);
    }
    //轨旁控制箱散热风扇
    if(0==ui->comboBox_4->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(1==ui->comboBox_4->currentIndex())
    {
        c = c | (0x02<<6);
    }
    ba[0] = c;c=0x00;

    //环境温度
    c = (char)ui->lineEdit->text().toInt();
    ba[1] = c;c=0x00;

    //DC5v电源电压
    ba[2] = ((ui->lineEdit_2->text().toUInt())>>8)&0x00FF;
    ba[3] = ((ui->lineEdit_2->text().toUInt())>>0)&0x00FF;

    obj.DataLen = ba.size();
    memcpy(obj.Data,ba.data(),ba.size());

    return obj;
}
