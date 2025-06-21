#ifndef PART1_LOGIN_SIGNUP_H
#define PART1_LOGIN_SIGNUP_H

#include "tcpclient.h"
#include "userstate.h"

#include <QWidget>
#include <QRegularExpression>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QMessageBox>
#include <QStackedWidget>
#include <QFile>
#include <QScroller>

namespace Ui {
class part1_login_signup;
}

class part1_login_signup : public QWidget
{
    Q_OBJECT

public:
    explicit part1_login_signup(TcpClient *client, QWidget *parent = nullptr);
    void move_page();
    void updateSignupButton();
    void fix_everything();

    void make_id();
    void move_page_to_login();
    void for_login();
    void move_page_to_search_pw();
    void move_page_to_search_id();

    QStackedWidget* getStackedWidget() const;
    QWidget* getSignupPage() const;
    QWidget* getLoginPage() const;

    int check_count = 0;

    ~part1_login_signup();

signals:
    void reqTurnToMain();

private:
    Ui::part1_login_signup *ui;
    TcpClient *client;
};

#endif // PART1_LOGIN_SIGNUP_H


