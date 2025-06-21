#ifndef PART6_ORDER_COMPLETE_H
#define PART6_ORDER_COMPLETE_H

#include <QWidget>
#include <QRegularExpression>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QMessageBox>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QFile>
#include <tcpclient.h>

namespace Ui {
class part6_order_complete;
}

class part6_order_complete : public QWidget
{
    Q_OBJECT

public:
    explicit part6_order_complete(TcpClient* client,QWidget *parent = nullptr);
    void on_myButton_clicked();
    void modify_clear();

    QString order_num = "";
    QString cooking_time = "";
    ~part6_order_complete();

signals:
    void reqToMainPage();

private:
    Ui::part6_order_complete *ui;
    TcpClient* client;
};

#endif // PART6_ORDER_COMPLETE_H
