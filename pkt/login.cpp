#include "login.h"
#include "ui_login.h"


login::login(TcpClient * client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login), client(client)
{
    ui->setupUi(this);

    connect(ui->join_btn, &QPushButton::clicked, this, &login::go_to_signup_screen);
    connect(ui->login_btn, &QPushButton::clicked, this, &login::do_login);    
}

void login::go_to_signup_screen(){
    emit req_turn_to_signup();
}

void login::go_to_dash_screen(){
    emit  req_turn_to_dash();
}

// 로그인기능
void login::do_login(){
    QJsonObject req;
    req["signal"]= "login";
    req["role"] = "store";
    req["biz_num"] ="1111";
    req["id"]=ui->id_le->text();
    req["pw"]=ui->pass_le->text();
    client->sendJson(req);

}

login::~login()
{
    delete ui;
}
