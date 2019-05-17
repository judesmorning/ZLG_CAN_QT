#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->init();
}

MainWindow::~MainWindow()
{
#if CLOSE_IF//这里析构没有用
    sendThreadCAN1.quit();//总的发送线程
    sendThreadCAN1.wait();
    parseThreadCAN1.quit();
    parseThreadCAN1.wait();
    sendThreadCAN2.quit();//总的发送线程
    sendThreadCAN2.wait();
    parseThreadCAN2.quit();
    parseThreadCAN2.wait();
    CanManager::GetInstance()->can1.setRcvWork(false);//2个接收线程
    CanManager::GetInstance()->can2.setRcvWork(false);
    CanManager::GetInstance()->can1.wait();
    CanManager::GetInstance()->can2.wait();
#endif
    delete ui;
}

/**
 *函数名:初始化
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::init()
{
    this->showMaximized();//最大化
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");//注册can结构体
    qRegisterMetaType<PVCI_CAN_OBJ>("PVCI_CAN_OBJ");//注册can结构体
    qRegisterMetaType<CAN_SEND_FRAME_STRUCT>("CAN_SEND_FRAME_STRUCT");

    this->setAttribute(Qt::WA_DeleteOnClose);

    //界面初始化
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//选中一行
    ui->tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    ui->tableWidget_3->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);//选中一行
    ui->tableWidget_3->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    //can1变量初始化
    QObject::connect(&timerCAN1,SIGNAL(timeout()),this,SLOT(timerOutCAN1Slot()));
    timerCAN1.setInterval(500);
    timerCAN1.start();
    can1TimersVc.resize(0);//can1用于循环发送的线程
    CanManager::GetInstance()->can1.moveToThread(&sendThreadCAN1);
    QObject::connect(&(CanManager::GetInstance()->can1),SIGNAL(sendedInfoSignal(QString)),this,SLOT(sendedInfoCAN1Slot(QString)));//can1发送的显示结果
    sendThreadCAN1.start();
    QObject::connect(&(CanManager::GetInstance()->can1),SIGNAL(getCanDataSignal(PVCI_CAN_OBJ,int)),&parseCAN1,SLOT(getCanDataSlot(PVCI_CAN_OBJ,int)));//can1->parse
    QObject::connect(&parseCAN1,SIGNAL(showInfoSignal(QString)),this,SLOT(rcvedInfoCAN1Slot(QString)));//parse->main
    parseCAN1.moveToThread(&parseThreadCAN1);
    parseThreadCAN1.start();
    QObject::connect(&mkPacketCAN1,SIGNAL(addSendFramSignal(CAN_SEND_FRAME_STRUCT)),this,SLOT(addSendFramCAN1Slot(CAN_SEND_FRAME_STRUCT)));
    can1SingalSendTimer = NULL;
    /*can2*/
    QObject::connect(&timerCAN2,SIGNAL(timeout()),this,SLOT(timerOutCAN2Slot()));
    timerCAN2.setInterval(500);
    timerCAN2.start();
    can2TimersVc.resize(0);//can2用于循环发送的线程
    CanManager::GetInstance()->can2.moveToThread(&sendThreadCAN2);
    QObject::connect(&(CanManager::GetInstance()->can2),SIGNAL(sendedInfoSignal(QString)),this,SLOT(sendedInfoCAN2Slot(QString)));//can2发送的显示结果
    sendThreadCAN2.start();
    QObject::connect(&(CanManager::GetInstance()->can2),SIGNAL(getCanDataSignal(PVCI_CAN_OBJ,int)),&parseCAN2,SLOT(getCanDataSlot(PVCI_CAN_OBJ,int)));//can1->parse
    QObject::connect(&parseCAN2,SIGNAL(showInfoSignal(QString)),this,SLOT(rcvedInfoCAN2Slot(QString)));//parse->main
    parseCAN2.moveToThread(&parseThreadCAN2);
    parseThreadCAN2.start();
    QObject::connect(&mkPacketCAN2,SIGNAL(addSendFramSignal(CAN_SEND_FRAME_STRUCT)),this,SLOT(addSendFramCAN2Slot(CAN_SEND_FRAME_STRUCT)));
    can2SingalSendTimer = NULL;
}

/**
 *函数名:打开can
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_clicked()
{
    int deviceTye,device,chanel;
    if(0==ui->comboBox->currentIndex())
    {
        deviceTye = 4;
    }
    else if(1==ui->comboBox->currentIndex())
    {
        deviceTye = 21;
    }
    else
    {
       QMessageBox::warning(this,"失败","错误的设备类型");
       return;
    }
    device = ui->lineEdit->text().toInt();
    if(0==ui->comboBox_2->currentIndex())
    {
         chanel = 0;
    }
    else if(1==ui->comboBox_2->currentIndex())
    {
        chanel = 1;
    }
    else if(2==ui->comboBox_2->currentIndex())
    {
        chanel = 2;
    }
    else
    {
        QMessageBox::warning(this,"失败","错误的通道号");
        return;
    }
    if(RET_ERR == CanManager::GetInstance()->openCan(deviceTye,device,chanel))
    {
        QMessageBox::warning(this,"失败","打开can失败");
        return;
    }
    QPalette palette;palette.setColor(QPalette::WindowText,Qt::red);
    ui->label_7->setPalette(palette);
    ui->label_7->setText("已连接");
}

/**
 *函数名:关闭can
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_2_clicked()
{
    int deviceTye,device;
    if(0==ui->comboBox->currentIndex())
    {
        deviceTye = 4;
    }
    else if(1==ui->comboBox->currentIndex())
    {
        deviceTye = 21;
    }
    else
    {
        QMessageBox::warning(this,"失败","错误的设备类型");
    }
    device = ui->lineEdit->text().toInt();
    if(RET_ERR == CanManager::GetInstance()->closeCan(deviceTye,device))
    {
        QMessageBox::warning(this,"失败","关闭can失败");
    }
    QPalette palette;palette.setColor(QPalette::WindowText,Qt::black);
    ui->label_7->setPalette(palette);
    ui->label_7->setText("未连接");
}

/**
 *函数名:获取can1发送信息
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::sendedInfoCAN1Slot(const QString &info)
{
    if(!ui->checkBox_5->isChecked())
    {
        return;
    }
    sendInfoCAN1 += info;
    sendInfoCAN1 += "\n";
}

/**
 *函数名:获取can1接收信息
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::rcvedInfoCAN1Slot(const QString &info)
{
    if(!ui->checkBox_4->isChecked())
    {
        return;
    }
    rcvInfoCAN1 += info;
    rcvInfoCAN1 += "\n";
}

/**
 *函数名:定时器回调
 *函数参数:NULL
 *函数作用:显示通信结果数据
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::timerOutCAN1Slot()
{
    if(ui->checkBox_4->isChecked())//显示can1接收的数据
    {
        if(rcvInfoCAN1.size()!=0)
        {
            ui->plainTextEdit->appendPlainText(rcvInfoCAN1.trimmed().toUpper());
            rcvInfoCAN1.clear();
        }
    }

    if(ui->checkBox_5->isChecked())//显示can1发送数据
    {
        if(sendInfoCAN1.size()!=0)
        {
            ui->plainTextEdit_2->appendPlainText(sendInfoCAN1.trimmed().toUpper());
            sendInfoCAN1.clear();
        }
    }
}

/***********************************************/
// z 函数名称:将要发送的can帧在table中显示
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
void MainWindow::addSendFramCAN1Slot(const CAN_SEND_FRAME_STRUCT &info)
{
#if CLOSE_IF
    qDebug()<<"接收到需要发送的can帧";
#endif
    ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
    ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,0,new QTableWidgetItem(info.packetTyepStr));
    ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,1,new QTableWidgetItem(info.idStr));
    ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,2,new QTableWidgetItem(info.dataStr));
    ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,3,new QTableWidgetItem(info.sourceStr));
    ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,4,new QTableWidgetItem(info.aimStr));
    ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,5,new QTableWidgetItem(info.timeStr));
    if(info.isSendToSelf)
    {
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,6,new QTableWidgetItem(SEND_SELF));
    }
    else
    {
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,6,new QTableWidgetItem(SEND_NORMAL));
    }
    ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,7,new QTableWidgetItem(QString::number(info.sendCount)));
    ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,8,new QTableWidgetItem(info.meaningStr));
    ui->tableWidget_2->resizeRowsToContents();
}

/**
 *函数名:can1添加帧
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_6_clicked()
{
    this->mkPacketCAN1.exec();
}

/***********************************************/
// z 函数名称:can1删除选中帧
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
void MainWindow::on_pushButton_7_clicked()
{
    ui->tableWidget_2->removeRow(ui->tableWidget_2->currentRow());
}

/***********************************************/
// z 函数名称:can1循环发送
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
void MainWindow::on_pushButton_8_clicked()
{
    if(can1TimersVc.size()!=0)
    {
        QMessageBox::warning(this,"错误","请先停止循环发送");
        return;
    }

    for(int i=0;i<ui->tableWidget_2->rowCount();i++)
    {
        can1TimersVc.push_back(new MyTimer());
        QObject::connect(can1TimersVc[i],SIGNAL(canSendSignal(VCI_CAN_OBJ)),&(CanManager::GetInstance()->can1),SLOT(sendSlot(VCI_CAN_OBJ)));
    }
    for(int i=0;i<(int)can1TimersVc.size();i++)
    {
        VCI_CAN_OBJ obj;
        memset(&obj,0,sizeof(obj));
        //ID
        obj.ID = ui->tableWidget_2->item(i,1)->text().toUInt(nullptr,Z_HEX);
        //发送类型
        if(SEND_SELF == ui->tableWidget_2->item(i,6)->text())
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
        obj.DataLen = ui->tableWidget_2->item(i,2)->text().remove(QRegExp("\\s")).size()/2;
        //数据
        QByteArray ba;
        QString tmpStr = ui->tableWidget_2->item(i,2)->text();
        RET_IF_NOT_EAQU(Mymethod::GetInstance()->getBytesFromQString(tmpStr,ba),RET_OK);
        memcpy(obj.Data,ba.data(),obj.DataLen);
        can1TimersVc[i]->doSendTimer(obj, ui->tableWidget_2->item(i,5)->text().toInt());
        can1TimersVc[i]->start();
    }
}

/**
 *函数名:can1单次发送
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_3_clicked()
{
    if(can1SingalSendTimer!=NULL)
    {
        if(can1SingalSendTimer->isRunning())
        {
            can1SingalSendTimer->quit();
            can1SingalSendTimer->wait();
        }
        delete can1SingalSendTimer;
        can1SingalSendTimer = NULL;
    }
    can1SingalSendTimer = new MyTimer(true);
    QObject::disconnect(can1SingalSendTimer,0,0,0);
    QObject::connect(can1SingalSendTimer,SIGNAL(canSendSignal(VCI_CAN_OBJ)),&(CanManager::GetInstance()->can1),SLOT(sendSlot(VCI_CAN_OBJ)));
    std::vector<VCI_CAN_OBJ> vcCan;
    std::vector<int> msleeps;
    for(int i=0;i<ui->tableWidget_2->rowCount();i++)//构造数据
    {
        for(int j=0;j<(ui->tableWidget_2->item(i,7)->text().toInt());j++)
        {
            VCI_CAN_OBJ obj;
            memset(&obj,0,sizeof(obj));
            //ID
            obj.ID = ui->tableWidget_2->item(i,1)->text().toUInt(nullptr,Z_HEX);
            //发送类型
            if(SEND_SELF == ui->tableWidget_2->item(i,6)->text())
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
            obj.DataLen = ui->tableWidget_2->item(i,2)->text().remove(QRegExp("\\s")).size()/2;
            //数据
            QByteArray ba;
            QString tmpStr = ui->tableWidget_2->item(i,2)->text();
            RET_IF_NOT_EAQU(Mymethod::GetInstance()->getBytesFromQString(tmpStr,ba),RET_OK);
            memcpy(obj.Data,ba.data(),obj.DataLen);
            vcCan.push_back(obj);
            msleeps.push_back(ui->tableWidget_2->item(i,5)->text().toInt());
        }
    }
    can1SingalSendTimer->setSingalSendData(vcCan,msleeps);
    can1SingalSendTimer->start();
}
/***********************************************/
// z 函数名称:can1停止发送
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
void MainWindow::on_pushButton_9_clicked()
{
    for(int i=0;i<(int)can1TimersVc.size();i++)
    {
        can1TimersVc[i]->stopSend();
        can1TimersVc[i]->wait();
        delete can1TimersVc[i];
    }
    std::vector<MyTimer*>().swap(can1TimersVc);
    can1TimersVc.resize(0);
}

/**
 *函数名:清空发送结果
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_10_clicked()
{
    ui->plainTextEdit_2->setPlainText("");
}

/**
 *函数名:清空接收结果
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_11_clicked()
{
    ui->plainTextEdit->setPlainText("");
}

/**
 *函数名:筛选报文类型
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    parseCAN1.setFilterType(index+1);
}

/**
 *函数名:筛选源地址
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    parseCAN1.setFilterSourAddr(index+1);
}

/**
 *函数名:是否打开id自增
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_checkBox_clicked(bool checked)
{
    auto_add_id = checked;
}

/**
 *函数名:获取can2发送信息
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::sendedInfoCAN2Slot(const QString &info)
{
    if(!ui->checkBox_6->isChecked())
    {
        return;
    }
    sendInfoCAN2 += info;
    sendInfoCAN2 += "\n";
}

/**
 *函数名:获取can2接收信息
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::rcvedInfoCAN2Slot(const QString &info)
{
    if(!ui->checkBox_7->isChecked())
    {
        return;
    }
    rcvInfoCAN2 += info;
    rcvInfoCAN2 += "\n";
}

/**
 *函数名:定时器回调
 *函数参数:NULL
 *函数作用:显示通信结果数据
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::timerOutCAN2Slot()
{
    if(ui->checkBox_7->isChecked())//显示can1接收的数据
    {
        if(rcvInfoCAN2.size()!=0)
        {
            ui->plainTextEdit_4->appendPlainText(rcvInfoCAN2.trimmed().toUpper());
            rcvInfoCAN2.clear();
        }
    }

    if(ui->checkBox_6->isChecked())//显示can1发送数据
    {
        if(sendInfoCAN2.size()!=0)
        {
            ui->plainTextEdit_3->appendPlainText(sendInfoCAN2.trimmed().toUpper());
            sendInfoCAN2.clear();
        }
    }
}

/***********************************************/
// z 函数名称:将要发送的can帧在table中显示
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
void MainWindow::addSendFramCAN2Slot(const CAN_SEND_FRAME_STRUCT &info)
{
#if CLOSE_IF
    qDebug()<<"接收到需要发送的can帧";
#endif
    ui->tableWidget_3->insertRow(ui->tableWidget_3->rowCount());
    ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,0,new QTableWidgetItem(info.packetTyepStr));
    ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,1,new QTableWidgetItem(info.idStr));
    ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,2,new QTableWidgetItem(info.dataStr));
    ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,3,new QTableWidgetItem(info.sourceStr));
    ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,4,new QTableWidgetItem(info.aimStr));
    ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,5,new QTableWidgetItem(info.timeStr));
    if(info.isSendToSelf)
    {
        ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,6,new QTableWidgetItem(SEND_SELF));
    }
    else
    {
        ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,6,new QTableWidgetItem(SEND_NORMAL));
    }
    ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,7,new QTableWidgetItem(QString::number(info.sendCount)));
    ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount()-1,8,new QTableWidgetItem(info.meaningStr));
    ui->tableWidget_3->resizeRowsToContents();
}

/**
 *函数名:can2添加帧
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_12_clicked()
{
    this->mkPacketCAN2.exec();
}

/**
 *函数名:can2删除帧
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_13_clicked()
{
    ui->tableWidget_3->removeRow(ui->tableWidget_3->currentRow());
}

/**
 *函数名:can2开始发送
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_14_clicked()
{
    if(can2TimersVc.size()!=0)
    {
        QMessageBox::warning(this,"错误","请先停止发送");
        return;
    }

    for(int i=0;i<ui->tableWidget_3->rowCount();i++)
    {
        can2TimersVc.push_back(new MyTimer());
        QObject::connect(can2TimersVc[i],SIGNAL(canSendSignal(VCI_CAN_OBJ)),&(CanManager::GetInstance()->can2),SLOT(sendSlot(VCI_CAN_OBJ)));
    }
    for(int i=0;i<(int)can2TimersVc.size();i++)
    {
        VCI_CAN_OBJ obj;
        memset(&obj,0,sizeof(obj));
        //ID
        obj.ID = ui->tableWidget_3->item(i,1)->text().toUInt(nullptr,Z_HEX);
        //发送类型
        if(SEND_SELF == ui->tableWidget_3->item(i,6)->text())
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
        obj.DataLen = ui->tableWidget_3->item(i,2)->text().remove(QRegExp("\\s")).size()/2;
        //数据
        QByteArray ba;
        QString tmpStr = ui->tableWidget_3->item(i,2)->text();
        RET_IF_NOT_EAQU(Mymethod::GetInstance()->getBytesFromQString(tmpStr,ba),RET_OK);
        memcpy(obj.Data,ba.data(),obj.DataLen);
        can2TimersVc[i]->doSendTimer(obj, ui->tableWidget_3->item(i,5)->text().toInt());
        can2TimersVc[i]->start();
    }
}

/**
 *函数名:can2单次发送
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_4_clicked()
{
    if(can2SingalSendTimer!=NULL)
    {
        if(can2SingalSendTimer->isRunning())
        {
            can2SingalSendTimer->quit();
            can2SingalSendTimer->wait();
        }
        delete can2SingalSendTimer;
        can2SingalSendTimer = NULL;
    }
    can2SingalSendTimer = new MyTimer(true);
    QObject::disconnect(can2SingalSendTimer,0,0,0);
    QObject::connect(can2SingalSendTimer,SIGNAL(canSendSignal(VCI_CAN_OBJ)),&(CanManager::GetInstance()->can2),SLOT(sendSlot(VCI_CAN_OBJ)));
    std::vector<VCI_CAN_OBJ> vcCan;
    std::vector<int> msleeps;
    for(int i=0;i<ui->tableWidget_3->rowCount();i++)//构造数据
    {
        for(int j=0;j<(ui->tableWidget_3->item(i,7)->text().toInt());j++)
        {
            VCI_CAN_OBJ obj;
            memset(&obj,0,sizeof(obj));
            //ID
            obj.ID = ui->tableWidget_3->item(i,1)->text().toUInt(nullptr,Z_HEX);
            //发送类型
            if(SEND_SELF == ui->tableWidget_3->item(i,6)->text())
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
            obj.DataLen = ui->tableWidget_3->item(i,2)->text().remove(QRegExp("\\s")).size()/2;
            //数据
            QByteArray ba;
            QString tmpStr = ui->tableWidget_3->item(i,2)->text();
            RET_IF_NOT_EAQU(Mymethod::GetInstance()->getBytesFromQString(tmpStr,ba),RET_OK);
            memcpy(obj.Data,ba.data(),obj.DataLen);
            vcCan.push_back(obj);
            msleeps.push_back(ui->tableWidget_3->item(i,5)->text().toInt());
        }
    }
    can2SingalSendTimer->setSingalSendData(vcCan,msleeps);
    can2SingalSendTimer->start();
}

/**
 *函数名:can2停止发送
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_15_clicked()
{
    for(int i=0;i<(int)can2TimersVc.size();i++)
    {
        can2TimersVc[i]->stopSend();
        can2TimersVc[i]->wait();
        delete can2TimersVc[i];
    }
    std::vector<MyTimer*>().swap(can2TimersVc);
    can2TimersVc.resize(0);
}

/**
 *函数名:can2清空发送
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_16_clicked()
{
    ui->plainTextEdit_3->setPlainText("");
}

/**
 *函数名:can2清空接收
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_pushButton_17_clicked()
{
    ui->plainTextEdit_4->setPlainText("");
}

/**
 *函数名:筛选can2报文类型
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_comboBox_5_currentIndexChanged(int index)
{
    parseCAN2.setFilterType(index+1);
}

/**
 *函数名:筛选can2源地址
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::on_comboBox_6_currentIndexChanged(int index)
{
    parseCAN2.setFilterSourAddr(index+1);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(0)
    {
        event->accept();
    }
    sendThreadCAN1.quit();//总的发送线程
    sendThreadCAN1.wait();
    parseThreadCAN1.quit();
    parseThreadCAN1.wait();
    sendThreadCAN2.quit();//总的发送线程
    sendThreadCAN2.wait();
    parseThreadCAN2.quit();
    parseThreadCAN2.wait();
    CanManager::GetInstance()->can1.setRcvWork(true);//2个接收线程
    CanManager::GetInstance()->can2.setRcvWork(true);
    CanManager::GetInstance()->can1.wait();
    CanManager::GetInstance()->can2.wait();
}




