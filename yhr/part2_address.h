#ifndef PART2_ADDRESS_H
#define PART2_ADDRESS_H

#include "tcpclient.h"
#include "w_v_addr_card.h"
#include "dataType.h"
#include "userstate.h"

#include <QWidget>
#include <QScroller>
#include <QScrollBar>
#include <QTimer>
#include <QButtonGroup>
#include <QBoxLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QStyle>
#include <QJsonArray>

namespace Ui {
class part2_address;
}

class part2_address : public QWidget
{
    Q_OBJECT

public:
    explicit part2_address(TcpClient* client, QWidget *parent = nullptr);

    void settingUI();
    void connectedPage();
    void registerAddress();
    void showAddrCard();

    ~part2_address();

signals:
    void reqTurnToMain();

private:
    Ui::part2_address *ui;
    TcpClient* client;
    w_v_addr_card* addr_card;
};

#endif // PART2_ADDRESS_H
