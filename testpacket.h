#ifndef TESTPACKET_H
#define TESTPACKET_H

#include <QDialog>
#include "packemanager.h"
namespace Ui {
class TestPacket;
}

class TestPacket : public QDialog , public PackeManager
{
    Q_OBJECT

public:
    explicit TestPacket(QWidget *parent = 0);
    ~TestPacket();
    VCI_CAN_OBJ getObj();
private:
    Ui::TestPacket *ui;
};

#endif // TESTPACKET_H
