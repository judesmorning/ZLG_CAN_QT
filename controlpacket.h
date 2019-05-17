#ifndef CONTROLPACKET_H
#define CONTROLPACKET_H

#include <QDialog>
#include "packemanager.h"
namespace Ui {
class ControlPacket;
}

class ControlPacket : public QDialog , public PackeManager
{
    Q_OBJECT

public:
    explicit ControlPacket(QWidget *parent = 0);
    ~ControlPacket();
    VCI_CAN_OBJ getObj();
private:
    Ui::ControlPacket *ui;
};

#endif // CONTROLPACKET_H
