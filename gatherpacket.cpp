#include "gatherpacket.h"
#include "ui_gatherpacket.h"

GatherPacket::GatherPacket(QWidget *parent) :
    QDialog(parent),
    PackeManager(GATHER_PACKET),
    ui(new Ui::GatherPacket)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
}

GatherPacket::~GatherPacket()
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
VCI_CAN_OBJ GatherPacket::getObj()
{
    memset(&obj,0,sizeof(obj));
    QByteArray ba;ba.resize(8);memset(ba.data(),0,ba.size());
    char c = 0x00;
    //车辆出入库信号
    if(0==ui->comboBox->currentIndex())//入
    {
        c = c | (0x01<<6);
    }
    else if(1==ui->comboBox->currentIndex())//出
    {
        c = c | (0x02<<6);
    }
    //测速传感器故障状态
    if(1==ui->comboBox_2->currentIndex())//故障
    {
        c = c | (0x01<<3);
    }
    if(1==ui->comboBox_3->currentIndex())//故障
    {
        c = c | (0x01<<2);
    }
    if(1==ui->comboBox_4->currentIndex())//故障
    {
        c = c | (0x01<<1);
    }
    if(1==ui->comboBox_5->currentIndex())//故障
    {
        c = c | (0x01<<0);
    }
    ba[0] = c;
    //车速值
    ba[1] = ((ui->lineEdit->text().toUInt())>>8)&0x00FF;
    ba[2] = ((ui->lineEdit->text().toUInt())>>0)&0x00FF;
    //系统时间
    ba[3] = ((ui->lineEdit_2->text().toUInt())>>24)&0x000000FF;
    ba[4] = ((ui->lineEdit_2->text().toUInt())>>16)&0x000000FF;
    ba[5] = ((ui->lineEdit_2->text().toUInt())>>8) &0x000000FF;
    ba[6] = ((ui->lineEdit_2->text().toUInt())>>0) &0x000000FF;
    obj.DataLen = ba.size();
    memcpy(obj.Data,ba.data(),ba.size());
    return obj;
}
