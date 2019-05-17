#include "makepacket.h"
#include "ui_makepacket.h"

MakePacket::MakePacket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakePacket)
{
    ui->setupUi(this);

    //报文名称初始化
    packetsName.resize(packetNum);
    packetsName[0] = "信号采集板采集报文";
    packetsName[1] = "设备间主控箱控制报文";
    packetsName[2] = "采集模块状态报文";
    packetsName[3] = "轨旁控制箱状态监测报文";
    packetsName[4] = "板卡信息报文";

    gatherPacket = new GatherPacket(this);
    controlPacket = new ControlPacket(this);
    stPacket = new StatePacket(this);
    testPacket = new TestPacket(this);
    infoPacket = new InfoPacket(this);

    on_comboBox_currentIndexChanged(0);
}

MakePacket::~MakePacket()
{
    delete ui;
}



/**
 *函数名:选择的报文变化回调
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MakePacket::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
    {
        controlPacket->hide();
        stPacket->hide();
        testPacket->hide();
        infoPacket->hide();
        this->setFixedHeight(gatherPacket->size().height()+100);
        gatherPacket->move(10,40);
        gatherPacket->show();

        break;
    }
    case 1:
    {
        gatherPacket->hide();
        stPacket->hide();
        testPacket->hide();
        infoPacket->hide();
        this->setFixedHeight(controlPacket->size().height()+100);
        controlPacket->move(10,40);
        controlPacket->show();

        break;
    }
    case 2:
    {
        gatherPacket->hide();
        controlPacket->hide();
        stPacket->hide();
        testPacket->hide();
        this->setFixedHeight(stPacket->size().height()+100);
        stPacket->move(10,40);
        stPacket->show();
        break;
    }
    case 3:
    {
        gatherPacket->hide();
        controlPacket->hide();
        stPacket->hide();
        infoPacket->hide();
        this->setFixedHeight(testPacket->size().height()+100);
        testPacket->move(10,40);
        testPacket->show();
        break;
    }
    case 4:
    {
        gatherPacket->hide();
        controlPacket->hide();
        stPacket->hide();
        testPacket->hide();
        this->setFixedHeight(infoPacket->size().height()+100);
        infoPacket->move(10,40);
        infoPacket->show();
        break;
    }
    default:
        break;
    }
}

/***********************************************/
// z 函数名称:生成报文
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
void MakePacket::on_pushButton_2_clicked()
{
    //获取id
    DWORD id = 0x00000000;
    int packetType = ui->comboBox->currentIndex()+1;
    int aimAddr = ui->comboBox_2->currentIndex()+1;
    if(aimAddr==6)
    {
        aimAddr = 31;
    }
    int sourAddr = ui->comboBox_3->currentIndex()+1;
    if(sourAddr==6)
    {
        sourAddr = 31;
    }
    id = id | ((packetType<<24)&0x1F000000);
    id = id | ((aimAddr<<16)&0x001F0000);
    id = id | ((sourAddr<<8)&0x00001F00);
    //获取data
    switch (ui->comboBox->currentIndex())
    {
    case 0://采集报文
    {
        VCI_CAN_OBJ obj = gatherPacket->getObj();
        QString idStr = QString("%1").arg(id,8,16,QLatin1Char('0'));
        ui->lineEdit->setText(idStr.toUpper());

        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,dataBa.size());
        QString dataStr = dataBa.toHex().toUpper();
        //qDebug()<<dataStr;
        RET_IF_NOT_EAQU(Mymethod::GetInstance()->addSpaceInQString(dataStr),RET_OK);
        ui->lineEdit_2->setText(dataStr.toUpper());

        break;
    }
    case 1://控制报文
    {
        VCI_CAN_OBJ obj = controlPacket->getObj();
        QString idStr = QString("%1").arg(id,8,16,QLatin1Char('0'));
        ui->lineEdit->setText(idStr.toUpper());

        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,dataBa.size());
        QString dataStr = dataBa.toHex().toUpper();
        //qDebug()<<dataStr;
        RET_IF_NOT_EAQU(Mymethod::GetInstance()->addSpaceInQString(dataStr),RET_OK);
        ui->lineEdit_2->setText(dataStr.toUpper());
        break;
    }
    case 2://状态报文
    {
        VCI_CAN_OBJ obj = stPacket->getObj();
        QString idStr = QString("%1").arg(id,8,16,QLatin1Char('0'));
        ui->lineEdit->setText(idStr.toUpper());

        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,obj.DataLen);
        QString dataStr = dataBa.toHex().toUpper();
        //qDebug()<<dataStr;
        RET_IF_NOT_EAQU(Mymethod::GetInstance()->addSpaceInQString(dataStr),RET_OK);
        ui->lineEdit_2->setText(dataStr.toUpper());
        break;
    }
    case 3://监测报文
    {
        VCI_CAN_OBJ obj = testPacket->getObj();
        QString idStr = QString("%1").arg(id,8,16,QLatin1Char('0'));
        ui->lineEdit->setText(idStr.toUpper());

        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,dataBa.size());
        QString dataStr = dataBa.toHex().toUpper();
        //qDebug()<<dataStr;
        RET_IF_NOT_EAQU(Mymethod::GetInstance()->addSpaceInQString(dataStr),RET_OK);
        ui->lineEdit_2->setText(dataStr.toUpper());
        break;
    }
    case 4://板卡信息
    {
        VCI_CAN_OBJ obj = infoPacket->getObj();
        QString idStr = QString("%1").arg(id,8,16,QLatin1Char('0'));
        ui->lineEdit->setText(idStr.toUpper());

        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,dataBa.size());
        QString dataStr = dataBa.toHex().toUpper();
        //qDebug()<<dataStr;
        RET_IF_NOT_EAQU(Mymethod::GetInstance()->addSpaceInQString(dataStr),RET_OK);
        ui->lineEdit_2->setText(dataStr.toUpper());
        break;
    }
    default:
        break;
    }

}

/***********************************************/
// z 函数名称:添加发送帧
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
void MakePacket::on_pushButton_clicked()
{
    VCI_CAN_OBJ obj;
    memset(&obj,0,sizeof(obj));
    //ID
    obj.ID = ui->lineEdit->text().toUInt(nullptr,Z_HEX);
    //发送类型
    if(ui->checkBox->isChecked())
    {
        obj.SendType = CAN_SEND_SELF;
    }
    else
    {
        obj.SendType = CAN_SEND_NORMAL;
    }
    //数据类型
    obj.RemoteFlag = CAN_DATA_INFO;
    //是否扩展帧
    obj.ExternFlag = CAN_FRAM_EXTERN;
    //数据长度
    obj.DataLen = ui->lineEdit_2->text().remove(QRegExp("\\s")).size()/2;
    //数据
    QByteArray ba;
    QString tmpStr = ui->lineEdit_2->text();
    RET_IF_NOT_EAQU(Mymethod::GetInstance()->getBytesFromQString(tmpStr,ba),RET_OK);
    memcpy(obj.Data,ba.data(),obj.DataLen);


    //构造发送给主界面显示的结构体
    CAN_SEND_FRAME_STRUCT info;
    //报文类型
    if( ui->comboBox->currentIndex() > (int)packetsName.size())
    {
        qDebug()<<"err in:"<<__FILE__<<" at:"<<__LINE__;
        return;
    }
    info.packetTyepStr = packetsName[ui->comboBox->currentIndex()];
    info.idStr = QString("%1").arg(obj.ID,8,16,QLatin1Char('0'));
    QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,obj.DataLen);
    info.dataStr = dataBa.toHex().toUpper();
    info.sourceStr = QString::number(Mymethod::GetInstance()->getSourceAdres(obj.ID));
    info.aimStr = QString::number(Mymethod::GetInstance()->getAimAdres(obj.ID));
    info.timeStr = QString::number(ui->lineEdit_3->text().toInt());
    if(0==ui->comboBox->currentIndex())
    {
        PackeManager pm(GATHER_PACKET);
        pm.setPacket(obj);
        info.meaningStr = pm.getMeaningByType();
    }
    else if(1==ui->comboBox->currentIndex())
    {
        PackeManager pm(CONTROL_PACKET);
        pm.setPacket(obj);
        info.meaningStr = pm.getMeaningByType();
    }
    else if(2==ui->comboBox->currentIndex())
    {
        PackeManager pm(STATE_PACKET);
        pm.setPacket(obj);
        info.meaningStr = pm.getMeaningByType();
    }
    else if(3==ui->comboBox->currentIndex())
    {
        PackeManager pm(TESTING_PACKET);
        pm.setPacket(obj);
        info.meaningStr = pm.getMeaningByType();
    }
    else if(4==ui->comboBox->currentIndex())
    {
        PackeManager pm(INFO_PACKET);
        pm.setPacket(obj);
        info.meaningStr = pm.getMeaningByType();
    }
    else
    {
        QMessageBox::warning(this,"错误","错误的报文类型");
    }
    if(ui->checkBox->isChecked())
    {
        info.isSendToSelf = true;
    }

    info.sendCount = (int)ui->lineEdit_4->text().toUInt();//发送次数
    emit this->addSendFramSignal(info);
}


