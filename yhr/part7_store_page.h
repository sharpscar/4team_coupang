#ifndef PART7_STORE_PAGE_H
#define PART7_STORE_PAGE_H

#include "dataType.h"
#include "tcpclient.h"
#include "w_v_menu_card.h"
#include "part8_menu_option_page.h"

#include "part5_cart.h"
#include "part6_order_complete.h"
#include "w_v_review_card.h"

#include <QWidget>
#include <QScrollBar>
#include <QLabel>
#include <QVector>
#include <QPushButton>
#include <QFile>

#include <QScroller>
#include <QString>
#include <QMessageBox>
#include <QRandomGenerator>


namespace Ui {
class part7_store_page;
}

class part7_store_page : public QWidget
{
    Q_OBJECT

public:
    explicit part7_store_page(QString store_name, TopData& data, TcpClient* client, QWidget *parent = nullptr);

    QString store_name;
    StoreInfo targetStore;
    QMap<QString, QLabel*> categoryLabelMap;
    QMap<QString, QPushButton*> stickyButtons;
    QStringList fixedKyochonOrder;
    QString normalizeKyochonCategory(const QString& raw);
    QVector <reviewInfo> reviews;

    void handleScrollChanged(int value);
    void showMenuCards();
    void scrollToCategory(const QString& categoryName);
    void settingStore();
    void settingUI();
    void showMSG(const QString& message, int ms);
    void collectConnect();
    void showAddrCard();

    ~part7_store_page();

signals:
    void reqBackCatPage();
    void reqMainPage();

private:
    Ui::part7_store_page *ui;
    TopData& data;
    w_v_menu_card* menu_card;
    part8_menu_option_page* option_page;
    TcpClient* client;
    part5_cart* cart_page;
    part6_order_complete* ordered_page;
    w_v_review_card* review_card;
};
#endif // PART7_STORE_PAGE_H
