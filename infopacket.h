#ifndef INFOPACKET_H
#define INFOPACKET_H

#include <QDialog>
#include "packemanager.h"
namespace Ui {
class InfoPacket;
}

class InfoPacket : public QDialog , public PackeManager
{
    Q_OBJECT

public:
    explicit InfoPacket(QWidget *parent = 0);
    ~InfoPacket();
    VCI_CAN_OBJ getObj();

private:
    Ui::InfoPacket *ui;
};

#endif // INFOPACKET_H
