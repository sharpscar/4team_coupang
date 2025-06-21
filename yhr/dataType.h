#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>
#include <QVector>


// 주소 정보
struct AddressInfo {
    QString addr_id;
    QString basic;      // 기본 주소
    QString detail;     // 상세 주소
    QString direction;  // 길안내
    QString type;       // 집, 회사, 기타
    bool isPrimary;     // 대표 주소 여부
};

// 장바구니 아이템
struct CartItem {
    QString menuName; // 메뉴 이름
    QString quantity; // 수량
    QString option;   // 옵션 정보
    QString price;    // 가격
};

// 장바구니 정보
struct CartInfo {
    QString storeName;          // 가게 이름
    QString eatType;            // 배달인지 포장인지
    QString totalPrice;         // 총 가격
    QVector<CartItem> items;    // 메뉴 목록
};

struct reviewInfo {
    QString userName;       // 유저 이름
    QString menu_name;      // 메뉴 이름
    QString avgStar;        // 평균 별점
    QString date;           // 날짜
    QString comment;        // 코멘트
    QString img_path;       // 이미지 경

};


enum CategoryType {
    간식 = 101,
    구이 = 102,
    도시락 = 103,
    돈까스 = 104,
    디저트 = 105,
    멕시칸 = 106,
    버거 = 107,
    분식 = 108,
    샌드위치 = 109,
    샐러드 = 110,
    아시안 = 111,
    양식 = 112,
    일식 = 113,
    족발_보쌈 = 114,
    죽 = 115,
    중식 = 116,
    찜_탕 = 117,
    치킨 = 118,
    커피 = 119, // 커피/차
    피자 = 120,
    한식 = 121,
    회_해물 = 122
};

struct OptionInfo {
    QString option_name;
    QString option_price;
    QString option_status;
};

struct MenuOption {
    QString option_category;
    QString option_required;
    QVector<OptionInfo> options;
};

struct MenuInfo {
    QString menu_category;
    QString menu_name;
    QString menu_info;
    QString menu_img_path;
    QString menu_status;
    QString menu_price;
    QVector<MenuOption> menu_options;
};

struct StoreInfo {
    QString store_name;
    int category_num;
    QString store_addr;
    QString phone_num;
    QString owner_name;
    QString company_num;
    QString opening_time;
    QString closing_time;
    QString delivery_fee;
    QString minimum_order;
    QString store_desc;
    QString origin_info;
    QString nutrition_fact;
    QString allergen_info;
    QString stataus;
    QVector<MenuInfo> menus;
};

struct Category {
    QString category_name;
    QVector<StoreInfo> restaurants;
};

struct TopData {
    QVector<Category> store_info;
};


#endif // DATATYPE_H
