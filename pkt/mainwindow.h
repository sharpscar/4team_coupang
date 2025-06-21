#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include "sign_up.h"
#include "dashboard.h"
#include "menumanage.h"
#include "ordercheck.h"
#include "timemanage.h"
#include "orderresdialog.h"
#include <QJsonArray>
#include <tcpclient.h>
#include <QLabel>

#include <QFile>
#include <QJsonDocument>
#include <QDateTime>

// 25.06.16 희라님이 만든 구조체들

// 메뉴 옵션 한 개
// struct OptionInfo {
//     QString option_name;     // 옵션 이름
//     int option_price;        // 옵션 가격
//     QString option_status;   // 옵션 상태
// };

// // 하나의 옵션 카테고리
// struct MenuOption {
//     QString option_category;      // 옵션 그룹 이름
//     QString option_required;      // 필수 여부 ("Y" 또는 "N")
//     QVector<OptionInfo> options;  // 이 옵션 그룹에 포함된 옵션들
// };

// // 하나의 메뉴 정보
// struct MenuInfo {
//     QString menu_category;             // 메뉴 카테고리 이름
//     QString menu_name;                 // 메뉴 이름
//     QString menu_info;                 // 메뉴 설명
//     QString menu_img_path;             // 이미지 경로
//     QVector<MenuOption> menu_options;  // 메뉴 옵션 그룹 목록
// };

// // 하나의 식당 정보
// struct StoreInfo {
//     QString store_name;       // 식당 이름
//     int category_num;         // 카테고리 번호
//     QString store_addr;       // 식당 주소
//     QString phone_num;        // 전화번호
//     QString owner_name;       // 사장님 이름
//     QString opening_time;     // 오픈 시간
//     QString closing_time;     // 마감 시간
//     QString delivery_fee;     // 배달 요금
//     QString minimum_order;    // 최소 주문 금액
//     QString store_desc;       // 가게 인사말
//     QString origin_info;      // 원산지 정보
//     QString nutrition_fact;   // 영양 성분
//     QString allergen_info;    // 알레르기 정보
//     QVector<MenuInfo> menus;  // 이 식당의 전체 메뉴 목록
// };


// struct Ordere_items {
//     QString menu_name;
//     QString quantity;
//     QString option;
//     QString price;
// };

// struct Orders{
//     QString order_serialNumber;
//     QString store_name;
//     QString order_status;
//     QString order_time;
//     QString to_owner;
//     QString to_rider;
//     QString user_addr;
//     QString user_pnum;
//     QString total_price;
//     QString eat_type;
//     QString once_plastic;
//     QVector<Ordere_items> order_items;
//     QString menu_quentity;
//     QString order_items_menu_names;
// };





QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initializeStackedWidget();
    void saveJsonToFile(const QJsonObject& json);
    void go_to_dash_screen();

    void get_menu_infomation(const QJsonObject& json);
    void get_order_infomation(const QJsonObject &json);
    void get_all_order_infomation(const QJsonObject& json);

signals:
    void riderInfoReceived(const QString& order_serialNumber, const QString& rider_info);       // 라이더 정보를 전달
    void completeInfoReceived(const QString& order_serialNumber);

private:
    Ui::MainWindow *ui;
    login *loginPage;
    sign_up *signupPage;
    Dashboard *dashboardPage;
    MenuManage *menuManagePage;
    Ordercheck *orderCheckPage;
    TimeManage *timeManagePage;
    OrderResDialog *orderDialogPage;
    TcpClient *client;
    QVector<Orders> orderList;



    Orders parseOrder(const QJsonObject& json);
    QMap<QString, Card*> m_cards;  //카드와 주문번호를 연결짓는 맵

    StoreInfo store;
    QVector<Orders> allOrders;
    Orders orders_;
    Order_items oi_;

};
#endif // MAINWINDOW_H
