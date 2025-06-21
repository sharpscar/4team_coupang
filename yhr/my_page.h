#ifndef MY_PAGE_H
#define MY_PAGE_H

#include <QWidget>

#include <QWidget>
#include <QRegularExpression>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QMessageBox>
#include <QStackedWidget>
#include <tcpclient.h>
#include <QMessageBox>
#include "userstate.h"


namespace Ui {
class my_page;
}

class my_page : public QWidget
{
    Q_OBJECT

public:
    explicit my_page(TcpClient *client, QWidget *parent = nullptr);

    void move_to_info_page();


    ~my_page();

private:
    Ui::my_page *ui;
    TcpClient *client;
};

#endif // MY_PAGE_H
