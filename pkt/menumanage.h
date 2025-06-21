#ifndef MENUMANAGE_H
#define MENUMANAGE_H

#include <QWidget>
#include "tcpclient.h"

#include <algorithm> // std::count_if
#include <QJsonArray>
#include <QJsonObject>
#include <QList>



// 메뉴 옵션 한 개
struct OptionInfo {
    QString option_name;     // 옵션 이름
    int option_price;        // 옵션 가격
    QString option_status;   // 옵션 상태
};

// 하나의 옵션 카테고리
struct MenuOption {
    QString option_category;      // 옵션 그룹 이름
    QString option_required;      // 필수 여부 ("Y" 또는 "N")
    QVector<OptionInfo> options;  // 이 옵션 그룹에 포함된 옵션들
};

// 하나의 메뉴 정보
struct MenuInfo {
    QString menu_category;             // 메뉴 카테고리 이름
    QString menu_name;                 // 메뉴 이름
    QString menu_info;                 // 메뉴 설명
    QString menu_img_path;             // 이미지 경로
    QVector<MenuOption> menu_options;  // 메뉴 옵션 그룹 목록
    QString menu_status;
    QString menu_price;
};

// 하나의 식당 정보
struct StoreInfo {
    QString store_name;       // 식당 이름
    int category_num;         // 카테고리 번호
    QString store_addr;       // 식당 주소
    QString phone_num;        // 전화번호
    QString owner_name;       // 사장님 이름
    QString opening_time;     // 오픈 시간
    QString closing_time;     // 마감 시간
    QString delivery_fee;     // 배달 요금
    QString minimum_order;    // 최소 주문 금액
    QString store_desc;       // 가게 인사말
    QString origin_info;      // 원산지 정보
    QString nutrition_fact;   // 영양 성분
    QString allergen_info;    // 알레르기 정보
    QVector<MenuInfo> menus;  // 이 식당의 전체 메뉴 목록
};





namespace Ui {
class MenuManage;
}



class MenuManage : public QWidget
{
    Q_OBJECT

public:
    explicit MenuManage(TcpClient * client, QWidget *parent = nullptr);
    ~MenuManage();
    void go_to_dash_screen();
    void go_to_ordercheck_screen();
    void go_to_timeManage_screen();
    void set_store_info(StoreInfo &store);
    void initialize_screen();

    void addMenu(); // 메뉴 추가 버튼

    //QJsonArray menusArr
    // menuManagePage.get_menus(menusArr);
    void updateMenu();
    void makeCategoryComboboxSetting();
    void onTableItemClicked(int row, int column);
    void deleteMenu();
signals:

    void req_turn_to_dashboard();
    void req_turn_to_ordercheck();
    void req_turn_to_timeManage();

private:
    Ui::MenuManage *ui;
    TcpClient * client;
    StoreInfo si;
    int all_menu_count;
    int menu_active_state;
    int menu_deactive_state;
    QList<MenuInfo> menuList;
    int row_index;
};

#endif // MENUMANAGE_H
