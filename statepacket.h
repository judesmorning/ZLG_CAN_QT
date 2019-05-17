#ifndef STATEPACKET_H
#define STATEPACKET_H

#include <QDialog>
#include "packemanager.h"


namespace Ui {
class StatePacket;
}

class StatePacket : public QDialog, public PackeManager
{
    Q_OBJECT

public:
    explicit StatePacket(QWidget *parent = 0);
    ~StatePacket();
    VCI_CAN_OBJ getObj();
private:
    Ui::StatePacket *ui;
signals:

};

#endif // STATEPACKET_H
