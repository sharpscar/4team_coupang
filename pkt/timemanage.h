#ifndef TIMEMANAGE_H
#define TIMEMANAGE_H

#include <QWidget>
#include "tcpclient.h"

#include <QHBoxLayout>
#include <QTimeEdit>

namespace Ui {
class TimeManage;
}

class TimeManage : public QWidget
{
    Q_OBJECT

public:
    explicit TimeManage(TcpClient * client, QWidget *parent = nullptr);
    ~TimeManage();
    void go_to_dash_screen();
    void go_to_ordercheck_screen();
    void go_to_timeManage_screen();
    void go_to_menuManage_screen();
signals:
    void req_turn_to_dashboard();
    void req_turn_to_ordercheck();
    void req_turn_to_timeManage();
    void req_turn_to_menuManage();


private:
    Ui::TimeManage *ui;
    TcpClient * client;
};

#endif // TIMEMANAGE_H
