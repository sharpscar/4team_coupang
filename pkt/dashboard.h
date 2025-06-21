#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include "tcpclient.h"

namespace Ui {
class Dashboard;
}

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(TcpClient *client,QWidget *parent = nullptr);
    ~Dashboard();
    void go_to_menu_screen();
    void go_to_time_screen();
    void go_to_order_screen();
    void do_logout();

signals:
    void req_turn_to_menu();
    void req_turn_to_login();
    void req_turn_to_order();
    void req_turn_to_timeManage();



private:
    Ui::Dashboard *ui;
    TcpClient * client;

};

#endif // DASHBOARD_H
