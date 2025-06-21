#ifndef ORDERCHECK_H
#define ORDERCHECK_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "tcpclient.h"
#include "card.h"
#include "QFile"
namespace Ui {
class Ordercheck;
}




struct Order_items {
    QString menu_name;
    QString quantity;
    QString option;
    QString price;
};

struct Orders{
    QString order_serialNumber;
    QString store_name;
    QString order_status;
    QString order_time;
    QString to_owner;
    QString to_rider;
    QString user_addr;
    QString user_pnum;
    QString total_price;
    QString eat_type;
    QString once_plastic;
    QVector<Order_items> order_Items;
    QString menu_quentity;
    QString order_items_menu_names;
};



class Ordercheck : public QWidget
{
    Q_OBJECT

public:
    explicit Ordercheck(TcpClient * client, QWidget *parent = nullptr);
    ~Ordercheck();
    void go_to_dash_screen();
    void go_to_ordercheck_screen();
    void go_to_timeManage_screen();
    void go_to_menuManage_screen();
    void fetchOrders();
    void connectToDatabase();
    void setCards();

    void add_order_info(Orders &orders);                 //  하나의 주문 정보를 prepend 하는 하는 함수
    void onDoneButtonClicked(QString sn);                // 조리완료 버튼을 눌렀을때 서버로 라이더를 요청하는 시그널을 보내는 함수
    void change_order_info(QVector<Orders> &allOrders_); // 서버로부터 들어온 완료된 오더를 저장한다.
    void saveJsonToFile(const QJsonObject& json);        //서버로부터 들어온 json을 파일로 갈무리 하는 함수

    void onRiderInfoReceived(const QString& order_serialNumber, const QString& rider_info); // 라이더 정보를 갱신
    void onCompleteInfoReceived(const QString& order_serialNumber);                         //완료정보




signals:
    void req_turn_to_dashboard();
    void req_turn_to_ordercheck();
    void req_turn_to_timeManage();
    void req_turn_to_menuManage();


private:
    Ui::Ordercheck *ui;
    TcpClient *client;


    // QVector<Ordere_items> v_items;


    Card *card;
    QMap<QString, Card*> m_cards;  //카드와 주문번호를 연결짓는 맵
    QVector<Orders> allOrders;
    Orders orders;
    Order_items oi;
    QString rider_p_num;
    QString rider_name;
    QString order_serialNumber;


};

#endif // ORDERCHECK_H
