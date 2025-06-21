#include "part7_store_page.h"
#include "ui_part7_store_page.h"


part7_store_page::part7_store_page(QString store_name, TopData& data, TcpClient* client, QWidget *parent)
    : QWidget(parent), store_name(store_name), ui(new Ui::part7_store_page), data(data), client(client)
{
    ui->setupUi(this);

    settingUI();
    settingStore();
    collectConnect();

    connect(client, &TcpClient::jsonReceived, this, [=](QJsonObject json) {
        QString signal = json["signal"].toString();

        if (signal == "send_reviews") {

            QJsonArray arr = json["reviews"].toArray();
            for (const QJsonValue& val : arr) {
                QJsonObject obj = val.toObject();

                reviewInfo review;
                review.userName = obj["user_name"].toString();
                review.menu_name = obj["menu_name"].toString();
                review.img_path = obj["img_path"].toString();
                review.avgStar = obj["star_rank"].toString();
                review.comment = obj["comment"].toString();
                review.date = obj["time"].toString();

                this->reviews.append(review);
            }

            ui->stackedWidget->setCurrentWidget(ui->review_page);
            showAddrCard();
        }
    });
}

void part7_store_page::showAddrCard() {
    // 기존 카드 제거
    QLayoutItem* item;
    while ((item = ui->layout_review_card->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    QRandomGenerator generator;

    QVector<reviewInfo> first;
    QVector<reviewInfo> rest;

    for (const reviewInfo& review : reviews) {
        if (review.date.contains("2025-06-21")) {
            first.push_back(review);
        } else {
            rest.push_back(review);
        }
    }

    // 6/21 생성되는 리뷰 먼저 보여주기
    QVector<reviewInfo> sortedReviews = first + rest;

    for (const reviewInfo& review : sortedReviews) {
        w_v_review_card* review_card = new w_v_review_card(this);

        review_card->findChild<QLabel*>("label_name")->setText(review.userName);
        int randomNumber = generator.bounded(100);
        review_card->findChild<QLabel*>("label_review_count")->setText("리뷰" + QString::number(randomNumber));
        review_card->findChild<QLabel*>("label_star")->setText("평균별점: " + review.avgStar);
        review_card->findChild<QLabel*>("label_date")->setText(review.date);
        review_card->findChild<QLabel*>("label_comment")->setText(review.comment);

        QLabel* imgLabel = review_card->findChild<QLabel*>("label_img");

        QString rawPath = review.img_path.trimmed();
        QString imgPath;

        if (!rawPath.isEmpty()) {
            if (rawPath.startsWith("./"))
                rawPath = rawPath.mid(2);

            imgPath = QString(":/%1").arg(rawPath);
            qDebug() << "최종 이미지 경로: " << imgPath;
        }


        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            QPixmap scaled = pix.scaled(350, 180);
            imgLabel->setPixmap(scaled);
            imgLabel->setScaledContents(true);
            imgLabel->setFixedSize(350, 180);
            // imgLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        } else {
            imgLabel->setText("이미지 없음");
        }

        ui->layout_review_card->addWidget(review_card);
    }
}



void part7_store_page::settingStore() {

    // 기존 카드 제거
    QLayoutItem* item;
    while ((item = ui->layout_menu_card->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    bool found = false;

    for (const Category& cat : data.store_info) {
        for (const StoreInfo& store : cat.restaurants) {
            if (store.store_name == store_name) {
                this->targetStore = store;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    if (!found) return;

    if(targetStore.store_name == "KYOCHON") {
        ui->label_store_name->setText("교촌치킨 소촌우산점");
    }
    else ui->label_store_name->setText(targetStore.store_name);
    ui->label_minimun->setText(targetStore.minimum_order);

    // store 이미지 경로 설정
    QString storeImgPath;
    if (store_name.contains("KYOCHON")) {
        storeImgPath = ":/STORE49_IMG/KYOCHON.jpg";  // 존재하는 실제 파일명으로 바꿔야 함
    } else {
        storeImgPath = ":/STORE49_IMG/" + targetStore.store_name +".jpg";
    }


    QPixmap storePix(storeImgPath);
    if (!storePix.isNull()) {
        int fixedWidth = 300;
        int fixedHeight = 200;

        QPixmap scaled = storePix.scaled(fixedWidth, fixedHeight, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        ui->store_img->setPixmap(scaled);
        ui->store_img->setAlignment(Qt::AlignCenter);
        ui->store_img->setFixedHeight(fixedHeight);

        QTimer::singleShot(0, this, [=]() {
            int btnSize = 26;
            int spacing = 5;
            int offset = 10;

            QStringList icons = {
                ":/mainIcon/brand_sdp_favorite_default.png",
                ":/mainIcon/btn_share.png",
                ":/mainIcon/main_topbar_bt_searchinput_icon.png"
            };

            for (int i = 0; i < icons.size(); ++i) {
                QPushButton* btn = new QPushButton(ui->store_img);
                btn->setIcon(QIcon(icons[i]));
                btn->setIconSize(QSize(btnSize - 6, btnSize - 6));
                btn->setFixedSize(btnSize, btnSize);
                btn->setStyleSheet(
                    "QPushButton {"
                    " background-color: white;"
                    " color: black;"
                    " border: none;"
                    " border-radius: 13px;"
                    "}"
                    );
                btn->move(ui->store_img->width() - (btnSize + spacing) * (i + 1) - offset, spacing);
                btn->raise();
                btn->show();

                connect(btn, &QPushButton::clicked, this, [=](){
                    QMessageBox::information(this, "즐겨찾기 완료", "교촌치킨 소촌우산점이 \n즐겨찾기에 정상 등록되었습니다!");

                    // ui->stackedWidget->setCurrentWidget(ui->login_page);
                });
            }
        });

    } else {
        ui->store_img->setText("가게 이미지 없음");
    }
}

void part7_store_page::handleScrollChanged(int value) {
    for (auto it = categoryLabelMap.begin(); it != categoryLabelMap.end(); ++it) {
        QLabel* label = it.value();
        int y = label->mapTo(ui->scrollArea_store->widget(), QPoint(0, 0)).y();
        if (value >= y - 20 && value < y + 200) { // 영역 안에 들어왔을 때
            QString currentCategory = it.key();
            scrollToCategory(currentCategory); // 강조 및 이동
            break;
        }
    }

    // 기존 stickyHeader 처리 유지
    int categoryBarY = ui->label_sticky->y();
    int headerShowY = 150;

    ui->stickyCategoryBar->setVisible(value >= categoryBarY);
    ui->stickyHeader->setVisible(value >= headerShowY);
}

void part7_store_page::showMenuCards() {

    if (targetStore.store_name.isEmpty()) return;

    QHBoxLayout* stickyLayout = ui->layoutStickyCategory;
    if (!stickyLayout) return;

    QVBoxLayout* menuLayout = ui->layout_menu_card;
    if (!menuLayout) return;

    QMap<QString, QVector<MenuInfo>> categorizedMenus;
    for (MenuInfo menu : this->targetStore.menus) {
        QString normalized = normalizeKyochonCategory(menu.menu_category);
        categorizedMenus[normalized].append(menu);
    }

    QStringList priorityOrder;
    for (const QString& key : categorizedMenus.keys()) {
        if (key.contains("사이드") || key.contains("주류") || key.contains("음료")) continue;
        priorityOrder << key;
    }
    for (const QString& key : categorizedMenus.keys()) {
        if (key.contains("사이드") || key.contains("주류") || key.contains("음료")) {
            priorityOrder << key;
        }
    }

    for (const QString& catName : fixedKyochonOrder) {
        if (!categorizedMenus.contains(catName)) continue;  // 해당 카테고리가 없으면 건너뜀
        const QVector<MenuInfo>& menus = categorizedMenus[catName];

        // 카테고리 버튼 추가 (stickyLayout에)
        QPushButton* stickyBtn = new QPushButton(catName);
        stickyLayout->addWidget(stickyBtn);
        stickyButtons[catName] = stickyBtn;
        connect(stickyBtn, &QPushButton::clicked, this, [=]() {
            scrollToCategory(catName);
        });

        // 카테고리 앵커
        QLabel* catAnchor = new QLabel;
        catAnchor->setFixedHeight(1);
        menuLayout->addWidget(catAnchor);
        categoryLabelMap[catName] = catAnchor;

        // 카테고리 제목
        QLabel* catTitle = new QLabel(catName);
        menuLayout->addWidget(catTitle);


        for (const MenuInfo& menu : menus) {
            w_v_menu_card* menuCard = new w_v_menu_card;
            menuCard->findChild<QLabel*>("m_name")->setText(menu.menu_name);
            menuCard->findChild<QLabel*>("m_price")->setText(menu.menu_price + "원");
            menuCard->findChild<QLabel*>("m_explain")->setText(menu.menu_info);

            QLabel* imgLabel = menuCard->findChild<QLabel*>("m_photo");

            connect(menuCard, &w_v_menu_card::clicked, this, [=](){
                if(UserState::getInstance().cart.storeName != "" && UserState::getInstance().cart.storeName != store_name){
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "같은 가게의 메뉴만 담을 수 있습니다.",
                                                  "주문할 가게를 변경하실 경우 이전에 담은 메뉴가 삭제됩니다.",
                                                  QMessageBox::Yes | QMessageBox::No);
                    if(reply == QMessageBox::No) {this->close();}
                    else {UserState::getInstance().clearCart();}
                }
                // 선택한 가게 이름&타입 담기
                UserState::getInstance().cart.storeName = store_name;
                UserState::getInstance().cart.eatType = "배달";

                part8_menu_option_page* option_page = new part8_menu_option_page(client, menu.menu_name, data, this);
                ui->stackedWidget->addWidget(option_page);
                ui->stackedWidget->setCurrentWidget(option_page);
                connect(option_page, &part8_menu_option_page::reqBackStorePage, this, [=](){
                    ui->stackedWidget->setCurrentWidget(ui->store_page);
                });

                connect(&UserState::getInstance(), &UserState::cartUpdated, this, [=](){
                    ui->stackedWidget->setCurrentWidget(ui->store_page);
                    ui->frame_show_cart->setVisible(true);
                    showMSG("메뉴가 담겼습니다.", 2000);

                    int total_quantity = 0;
                    for (const CartItem& item : UserState::getInstance().cart.items) {
                        total_quantity += item.quantity.toInt();
                    }
                    QString cart_info = QString::number(total_quantity) + " " + "카트 보기    " + UserState::getInstance().cart.totalPrice + " 원";
                    ui->btn_show_cart->setText(cart_info);

                    connect(ui->btn_show_cart, &QPushButton::clicked, this, [=](){
                        part5_cart* cart_page = new part5_cart(client, this);
                        ui->stackedWidget->addWidget(cart_page);
                        ui->stackedWidget->setCurrentWidget(cart_page);

                        connect(cart_page, &part5_cart::sucessOrder, this, [=](){
                            part6_order_complete* ordered_page = new part6_order_complete(client, this);
                            ui->stackedWidget->addWidget(ordered_page);
                            ui->stackedWidget->setCurrentWidget(ordered_page);

                            connect(ordered_page, &part6_order_complete::reqToMainPage, this, [=](){
                                emit reqMainPage();
                            });
                        });
                    });
                });
            });


            if (!menu.menu_img_path.isEmpty()) {

                QString relativePath = menu.menu_img_path;
                if (relativePath.startsWith("./")) relativePath = relativePath.mid(2);
                if (relativePath.startsWith("KYOCHON_IMG/")) relativePath = relativePath.mid(QString("KYOCHON_IMG/").length());

                QString imgPath;
                if (store_name.toUpper().contains("KYOCHON")) {
                    imgPath = QString(":/KYOCHON_IMG/%1").arg(relativePath);
                } else {
                    imgPath = QString(":/STORE49_IMG/%1").arg(relativePath);
                }

                QPixmap pix(imgPath);
                if (!pix.isNull()) {
                    int fixedWidth = 120;
                    int fixedHeight = 120;
                    QPixmap scaled = pix.scaledToWidth(fixedWidth, Qt::SmoothTransformation);
                    imgLabel->setPixmap(scaled);
                    imgLabel->setFixedWidth(fixedWidth);  // 고정 가로
                    imgLabel->setFixedHeight(fixedHeight); // 고정 세로
                    imgLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);  // 세로는 내용에 따라
                } else {
                    imgLabel->setText("이미지 없음");  // 기본 이미지 사용 가능
                }
            }

            menuLayout->addWidget(menuCard);
        }
    }
}

void part7_store_page::scrollToCategory(const QString& categoryName) {
    if (categoryLabelMap.contains(categoryName)) {
        QLabel* targetLabel = categoryLabelMap[categoryName];
        int y = targetLabel->mapTo(ui->scrollArea_store->widget(), QPoint(0, 0)).y();
        ui->scrollArea_store->verticalScrollBar()->setValue(y - 80);
    }

    for (auto btn : stickyButtons) {
        btn->setStyleSheet("padding: 8px; font-size: 13px;");
    }

    if (stickyButtons.contains(categoryName)) {
        QPushButton* btn = stickyButtons[categoryName];
        btn->setStyleSheet("padding: 8px; font-size: 13px; background-color: #1976d2; color: white;");

        // 가운데 정렬 (수평 스크롤 영역)
        int x = btn->x() + btn->width() / 2;
        QScrollBar* hbar = ui->scrollArea_category->horizontalScrollBar();
        hbar->setValue(x - ui->scrollArea_category->width() / 2);
    }
}

QString part7_store_page::normalizeKyochonCategory(const QString& raw) {
    QString name = raw.trimmed();  // 공백 제거

    static QMap<QString, QString> mapping = {
                                             { "인기", "인기메뉴" },
                                             { "윙박스", "윙박스시리즈" },
                                             { "싱글윙", "싱글윙시리즈" },
                                             { "허니", "허니시리즈" },
                                             { "후라이드", "후라이드시리즈" },
                                             { "허니옥수수", "허니옥수수" },
                                             { "레드", "레드시리즈" },
                                             { "간장", "간장시리즈" },
                                             { "반반", "반반(믹스)시리즈" },
                                             { "살살", "살살시리즈" },
                                             { "반마리", "반마리시리즈" },
                                             { "사이드", "사이드메뉴" },
                                             { "소스", "소스 및 무" },
                                             { "무", "소스 및 무" },
                                             { "음료", "음료 주류" },
                                             { "주류", "음료 주류" },
                                             };

    for (const QString& key : mapping.keys()) {
        if (name.contains(key)) {
            return mapping[key];
        }
    }

    return name;
}

void part7_store_page::settingUI() {

    // 스크롤 영역에서 마우스 제스처로 옆으로 움직일 수 있도록 설정
    QScroller::grabGesture(ui->scrollArea_review->viewport(), QScroller::LeftMouseButtonGesture);
    // 스크롤 비활성화
    ui->scrollArea_review->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_review->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->frame_show_cart->setVisible(false);

    this->fixedKyochonOrder = {
        "인기메뉴",
        "윙박스시리즈",
        "싱글윙시리즈",
        "허니시리즈",
        "후라이드시리즈",
        "허니옥수수",
        "레드시리즈",
        "간장시리즈",
        "반반(믹스)시리즈",
        "살살시리즈",
        "반마리시리즈",
        "사이드메뉴",
        "소스 및 무",
        "음료 주류"
    };

    ui->stickyHeader->setVisible(false);
    ui->stickyCategoryBar->setVisible(false);
    ui->line_search_menu->setPlaceholderText("교촌치킨 소촌우산점 내 메뉴를 찾아보세요");

    QScroller::grabGesture(ui->scrollArea_category->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_store->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scroll_review->viewport(), QScroller::LeftMouseButtonGesture);
    ui->scrollArea_category->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_category->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_store->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_store->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scroll_review->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scroll_review->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

// 검정색 하단 메세지 박스
void part7_store_page::showMSG(const QString& message, int ms) {
    QLabel* msg = new QLabel(message, this);  // this는 MainWindow
    msg->setStyleSheet("background-color: rgba(0, 0, 0, 180); color: white; padding: 10px; border-radius: 10px;");
    msg->setAlignment(Qt::AlignCenter);
    msg->setFixedWidth(336);
    msg->setFixedHeight(40);
    msg->move((width() - msg->width() - 20), height()-110);
    msg->show();

    QTimer::singleShot(ms, msg, [=]() {
        msg->close();
        msg->deleteLater();
    });
}

void part7_store_page::collectConnect() {
    connect(ui->btnBackToCatPage, &QPushButton::clicked, this, [=](){
        emit reqBackCatPage();
    });
    connect(ui->btn_togo, &QPushButton::clicked, this, [=](){
        emit reqBackCatPage();
    });

    connect(ui->scrollArea_store->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &part7_store_page::handleScrollChanged);
    showMenuCards();

    connect(&UserState::getInstance(), &UserState::orderComplete, this, [=](){
        ui->frame_show_cart->setVisible(false);
    });
    connect(ui->btn_review, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->review_page);

        QJsonObject req;
        req["signal"] = "get_reviews";
        req["role"] = "user";
        req["store_name"] = "교촌치킨";
        // 리뷰 요청하기
        client->sendJson(req);
    });

    connect(ui->BtnBackToStorePage, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->store_page);
    });

}

part7_store_page::~part7_store_page()
{
    delete ui;
}
