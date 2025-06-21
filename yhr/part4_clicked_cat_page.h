#ifndef PART4_CLICKED_CAT_PAGE_H
#define PART4_CLICKED_CAT_PAGE_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QRandomGenerator>
#include <QScroller>
#include <QScrollBar>
#include "dataType.h"
#include "w_v_store_card.h"


namespace Ui {
class part4_clicked_cat_page;
}

class part4_clicked_cat_page : public QWidget
{
    Q_OBJECT

public:
    explicit part4_clicked_cat_page(QString name, TopData& data, QWidget *parent = nullptr);
    QString name;
    QStringList categories = {"신규 맛집", "치킨", "커피/차", "버거", "구이", "샐러드",
                             "한식", "분식", "돈까스", "족발/보쌈", "찜/탕",
                             "피자", "중식", "일식", "회/해물", "양식",
                             "디저트", "간식", "아시안", "샌드위치", "멕시칸",
                             "도시락", "죽", "포장", "1인분", "프랜차이즈"};
    void settinggUI();
    void showCategoriesBtn(QStringList categories);
    void showStoreCards();

    ~part4_clicked_cat_page();

signals:
    void reqToMainPage();
    void categoryClicked(QString name);
    void storeCardClicked(QString store_name);

private:
    Ui::part4_clicked_cat_page *ui;
    w_v_store_card* store_card;
    TopData& data;
};

#endif // PART4_CLICKED_CAT_PAGE_H
