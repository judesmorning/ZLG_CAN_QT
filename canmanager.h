#ifndef CANMANAGER_H
#define CANMANAGER_H

#include "includes.h"
#include "ControlCAN.h"
#include "mymethod.h"
#include "mycanthread.h"
class CanManager
{
private:
        CanManager();
public:
    static CanManager *GetInstance();
public:
    int openCan(int deviceTye,int device,int chanel);
    int closeCan(int deviceTye,int device);
public:
    MyCanThread can1,can2;//两个can通道
};

#endif // CANMANAGER_H
