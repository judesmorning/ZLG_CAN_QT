#include "mytimer.h"

bool auto_add_id=false;

MyTimer::MyTimer(bool isSendOneTime) :
    QThread()
{
    this->isSendOneTime = isSendOneTime;
}
/***********************************************/
// z 函数名称:NULL
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
int MyTimer::doSendTimer(const VCI_CAN_OBJ &info, const int time)
{
    int ret = RET_OK;
    if(time <= 0)
    {
        ret = RET_ERR;
    }
    this->obj = info;
    this->time = time;

    return ret;
}

/**
 *函数名:设置单次发送的数据
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
int MyTimer::setSingalSendData(const std::vector<VCI_CAN_OBJ> &vcCan, std::vector<int> &msleeps)
{
    int ret = RET_OK;
    if(vcCan.size()!=(msleeps.size()))
    {
        ret = RET_ERR;
    }
    else
    {
        this->vcCan = vcCan;
        this->msleeps = msleeps;
    }
    return ret;
}

/***********************************************/
// z 函数名称:线程函数
// h 函数作用:NULL
// u 函数参数:NULL
// x 函数返回值:NULL
// y 备注:NULL
/***********************************************/
void MyTimer::run()
{
    if(!isSendOneTime)//循环发送
    {
        if(this->time<=0)
        {
            stop==false;
        }
        unsigned char tmpHeart=0;
        while(!stop)
        {
            if(auto_add_id)//是否自增心跳
            {
                tmpHeart = (unsigned char)Mymethod::GetInstance()->getHeart(obj.ID);
                tmpHeart++;
                obj.ID = (obj.ID & 0xFFFFFF00) | tmpHeart;
            }
            emit canSendSignal(this->obj);
            this->msleep(this->time);
        }
    }
    else//单词发送
    {
        if(vcCan.size()>0)
        {
            int count = vcCan.size();
            int i=0;
            while(count > i)
            {
                VCI_CAN_OBJ tmpObj = vcCan[i];
                emit canSendSignal(tmpObj);
                this->msleep(msleeps[i]);
                i++;
            }
        }
        isSendOneTime = false;
        std::vector<VCI_CAN_OBJ> vcTemp1;vcTemp1.swap(vcCan);
        std::vector<int> vcTemp2;vcTemp2.swap(msleeps);
    }

}
