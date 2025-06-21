#include "dashboard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(TcpClient * client,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Dashboard), client(client)
{
    ui->setupUi(this);

    connect(ui->btn_menu, &QPushButton::clicked, this, &Dashboard::go_to_menu_screen);
    connect(ui->btn_logout, &QPushButton::clicked, this, &Dashboard::do_logout);
    connect(ui->btn_order, &QPushButton::clicked, this, &Dashboard::go_to_order_screen);
    connect(ui->btn_time, &QPushButton::clicked, this, &Dashboard::go_to_time_screen);

}

void Dashboard::go_to_menu_screen()
{
    emit req_turn_to_menu();
}
void Dashboard::go_to_order_screen(){
    emit req_turn_to_order();
}

void Dashboard::go_to_time_screen(){    
    emit req_turn_to_timeManage();
}

void Dashboard::do_logout(){
    emit req_turn_to_login();
}

Dashboard::~Dashboard()
{
    delete ui;
}
