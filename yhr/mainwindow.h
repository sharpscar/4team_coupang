#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpclient.h"
#include "part1_login_signup.h"
#include "w_grid_category.h"
#include "w_h_category.h"


#include "dataType.h"
#include "w_v_store_card.h"
#include "my_page.h"
#include "part2_address.h"
#include "part3_search.h"
#include "part4_clicked_cat_page.h"
#include "part5_cart.h"
#include "part6_order_complete.h"
#include "part7_store_page.h"
#include "part9_write_review.h"
#include "part10_favorite.h"

#include <QMainWindow>
#include <QDialog>
#include <QTimer>
#include <QFile>

#include <QGesture>
#include <QGestureEvent>
#include <QScrollBar>
#include <QScroller>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QRandomGenerator>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>


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

    void SettingCard();
    void showChoiceLoginDialg();
    void categoryBtn();

    // 초기화 함수
    void init_scrollbar();
    // 초기 세팅
    void setting_main();
    // 스크롤바의 위치가 100이 넘어가면 생기는 업 버튼
    void show_up_btn();
    // 서버에서 온 json 처리 함수
    void process_json_form_server();
    void handleCategoryClicked(QString name);
    TopData parseTopData(const QJsonArray& storeInfoArr);
    // json 파일 생성
    void saveJsonToFile(const QJsonObject& json);
    void addWidgetLastLayout();
    void showMSG(const QString& message, int ms);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TcpClient* client;

    part1_login_signup* login;
    my_page* my_eats_page;
    w_grid_category* category;
    w_v_store_card* store_card;
    w_h_category* h_category;

    part2_address* addr_page;
    part3_search* search_page;
    part4_clicked_cat_page* cat_page;
    part5_cart *cart_page;
    part6_order_complete *ordered_page;
    part7_store_page* store_page;

    part9_write_review *write_review;
    part10_favorite *favorite_page;
    QTimer *adTimer;
    int currentAdIndex = 0;
    // 파싱한 전체 데이터 보관
    TopData topData;
};
#endif // MAINWINDOW_H

