#include "infopacket.h"
#include "ui_infopacket.h"

InfoPacket::InfoPacket(QWidget *parent) :
    QDialog(parent),
    PackeManager(INFO_PACKET),
    ui(new Ui::InfoPacket)
{
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->setupUi(this);
}

InfoPacket::~InfoPacket()
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
VCI_CAN_OBJ InfoPacket::getObj()
{
    memset(&obj,0,sizeof(obj));
    QByteArray ba;ba.resize(8);memset(ba.data(),0,ba.size());
    char c = 0x00;
    //生命信号
    ba[0] = (unsigned char)ui->lineEdit->text().toUInt();
    //硬件主版本号
    ba[1] = (unsigned char)ui->lineEdit_2->text().toUInt();
    //硬件次版本号
    ba[2] = (unsigned char)ui->lineEdit_3->text().toUInt();
    //软件主版本号
    ba[3] = (unsigned char)ui->lineEdit_4->text().toUInt();
    //软件次版本号
    ba[4] = (unsigned char)ui->lineEdit_5->text().toUInt();

    //can2故障状态
    if(0==ui->comboBox_2->currentIndex())
    {
        c = c | (0x01<<4);
    }
    else if(1==ui->comboBox_2->currentIndex())
    {
        c = c | (0x02<<4);
    }
    //can1故障状态
    if(0==ui->comboBox->currentIndex())
    {
        c = c | (0x01<<6);
    }
    else if(1==ui->comboBox->currentIndex())
    {
        c = c | (0x02<<6);
    }
    ba[5] = c;c=0x00;

    obj.DataLen = ba.size();
    memcpy(obj.Data,ba.data(),ba.size());
    return obj;
}
