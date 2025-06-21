#ifndef SIGN_UP_H
#define SIGN_UP_H

#include "tcpclient.h"

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class sign_up;
}

class sign_up : public QWidget
{
    Q_OBJECT

public:
    explicit sign_up(TcpClient * client, QWidget *parent = nullptr);

    //.h
    // 시/도, 구군시 콤보 박스 세팅 함수
    void setRegion(QMap<QString, QStringList>& regionMap);
    void do_check_duplicate();

    ~sign_up();

signals:
    //회원가입 신호를 듣고 페이지변환 메인으로 보낸다.
    void req_turn_to_login();


private:
    Ui::sign_up *ui;
    void go_to_login_screen();
    TcpClient* client ;
    void do_store_signup();
    void reset_input_fields();
};

#endif // SIGN_UP_H
