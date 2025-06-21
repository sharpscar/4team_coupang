#ifndef LOGIN_H
#define LOGIN_H

#include "tcpclient.h"

#include <QWidget>
#include <QMessageBox>
namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(TcpClient * client, QWidget *parent = nullptr);
    ~login();
    void do_login();

    void go_to_dash_screen();
    void go_to_signup_screen();
private:
    Ui::login *ui;
    TcpClient * client;

signals:
    // 회원 가입 버튼을 누르면 회원가입 페이지로 이동 하는 신호
    void req_turn_to_signup();
    void req_turn_to_main();
    // 로그인후 대시보드 스크린으로 이동 하는 신호
    void req_turn_to_dash();
};

#endif // LOGIN_H
