#include "part4_clicked_cat_page.h"
#include "ui_part4_clicked_cat_page.h"

part4_clicked_cat_page::part4_clicked_cat_page(QString name, TopData& data, QWidget *parent)
    : QWidget(parent), name(name), ui(new Ui::part4_clicked_cat_page), data(data)
{
    ui->setupUi(this);
    ui->scroll_store_cards->setWidgetResizable(true);

    settinggUI();
    showCategoriesBtn(this->categories);
    // 하단 식당 카드 리스트
    showStoreCards();


    // 메인화면으로 돌아가기
    connect(ui->BtnBackToMain, &QPushButton::clicked, this, [=](){
        emit reqToMainPage();
    });
}


void part4_clicked_cat_page::settinggUI() {
    ui->label_cat_name->setText(name);
    // 하단의 스크롤바 없애기
    ui->scroll_h_cat_names->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scroll_store_cards->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 제스터를 이용해 옆으로 움직이게 하기
    QScroller::grabGesture(ui->scroll_h_cat_names->viewport(), QScroller::LeftMouseButtonGesture);

    // ※ 우선 브랜드 스크롤 영역은 숨기고 우선적인 기능들 구현한 후 추가하자! ※
    ui->scroll_brands->hide();
}


void part4_clicked_cat_page::showCategoriesBtn(QStringList categories){
    ui->layout_cat_names->setContentsMargins(0, 0, 0, 0);
    ui->layout_cat_names->setSpacing(0);

    for (int i = 0; i < categories.size(); i++) {
        QString categoryName = categories[i];

        QPushButton* btn = new QPushButton;
        btn->setText(categoryName);
        btn->setMinimumWidth(80);
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        // 선택된 카테고리 파란색 처리
        if (categoryName == name) {
            btn->setStyleSheet("color: #00A9FF; font-weight: bold; border-bottom: 2px solid blue;");

            // 버튼이 scrollArea 왼쪽으로 오도록
            QTimer::singleShot(0, this, [=]() {
                ui->scroll_h_cat_names->horizontalScrollBar()->setValue(btn->x());
            });
        } else {
            btn->setStyleSheet("font-weight: bold;");
        }

        // 버튼 클릭 시 시그널 발생
        connect(btn, &QPushButton::clicked, this, [=](){
            emit categoryClicked(categoryName);
        });

        ui->layout_cat_names->addWidget(btn);
    }
}


void part4_clicked_cat_page::showStoreCards() {
    // 기존 카드 제거
    QLayoutItem* item;
    while ((item = ui->layout_store_cards->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    // name에 해당하는 카테고리 이름 찾기
    for (const Category& cat : data.store_info) {
        if (cat.category_name == name) {
            for (const StoreInfo& store : cat.restaurants) {
                w_v_store_card* store_card = new w_v_store_card(this);

                // UI 세팅
                store_card->findChild<QLabel*>("store_name")->setText(store.store_name);
                if(store.store_name == "KYOCHON") {
                    store_card->findChild<QLabel*>("store_name")->setText("교촌치킨 소촌우산점");
                }
                store_card->findChild<QLabel*>("free_delivery")->setText("무료배달");
                store_card->findChild<QLabel*>("mini_order")->setText("최소주문 " + store.minimum_order);
                store_card->findChild<QLabel*>("es_stime")->setText(store.opening_time + " ~ " + store.closing_time);


                QLabel* distanceLabel = store_card->findChild<QLabel*>("distance");
                QLabel* reviewLabel = store_card->findChild<QLabel*>("review");

                if (reviewLabel) {
                    if (store.store_name.contains("KYOCHON")) {
                        reviewLabel->setText("⭐ 4.7");
                    } else {
                        int randInt = QRandomGenerator::global()->bounded(9);
                        float score = 4.2f + 0.1f * randInt;
                        reviewLabel->setText("⭐ " + QString::number(score, 'f', 1));
                    }
                }


                if (distanceLabel) {
                    if (store.store_name.contains("KYOCHON")) {
                        distanceLabel->setText("0.2km");
                    } else {
                        int randInt = QRandomGenerator::global()->bounded(28);
                        float dist = 0.3f + 0.1f * randInt;
                        distanceLabel->setText(QString::number(dist, 'f', 1) + "km");
                    }
                }


                // 이미지 세팅
                if (!store.menus.isEmpty()) {
                    QLabel* imgLabel = store_card->findChild<QLabel*>("food_picture");

                    QString path;
                    if (store.store_name == "KYOCHON") {
                        path = "/home/y/CouCouEatsUser/STORE49_IMG/KYOCHON.jpg";
                    } else {
                        path = store.menus[0].menu_img_path;
                        if (!path.startsWith(":/"))  // 리소스 경로 보정
                            path = ":/" + path;
                    }

                    QPixmap pix(path);
                    if (imgLabel && !pix.isNull()) {
                        imgLabel->setPixmap(pix.scaled(350, 250, Qt::KeepAspectRatio));

                    } else {
                        qDebug() << "이미지 로딩 실패:" << path;
                    }
                }

                // 버튼을 누르면 MainWindow에 신호 보내기
                connect(store_card, &w_v_store_card::clicked, this, [=](){
                    emit storeCardClicked(store.store_name);
                });

                // 카드 위젯 추가
                ui->layout_store_cards->addWidget(store_card);
            }
            break;
        }
    }
}



part4_clicked_cat_page::~part4_clicked_cat_page() {
    delete ui;
}
