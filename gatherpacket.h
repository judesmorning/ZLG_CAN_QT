#ifndef GATHERPACKET_H
#define GATHERPACKET_H

#include <QDialog>
#include "packemanager.h"
namespace Ui {
class GatherPacket;
}

class GatherPacket : public QDialog , public PackeManager
{
    Q_OBJECT

public:
    explicit GatherPacket(QWidget *parent = 0);
    ~GatherPacket();

    VCI_CAN_OBJ getObj();
private:
    Ui::GatherPacket *ui;
};

#endif // GATHERPACKET_H
