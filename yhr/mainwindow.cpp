#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ui->btn_search->hide();
    setting_main();
    process_json_form_server();

    connect(&UserState::getInstance(), &UserState::orderComplete, this, [=](){
        ui->cart_frame->setVisible(false);
    });

    connect(&UserState::getInstance(), &UserState::loginStatusChanged, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_main);
        UserState::getInstance().isLoggedIn = true;
        showMSG("로그인 되었습니다. 쿠팡이츠 맛집을 둘러보세요!", 2000);;
    });

    connect(&UserState::getInstance(), &UserState::log_out, this, [=](){
        this->close();
    });
    // 알람 버튼
    connect(ui->btn_alarm, &QPushButton::clicked, this, [=](){

    });

    connect(ui->start, &QPushButton::clicked, this, [=]() {
        // 현재 페이지에 투명도 효과
        QGraphicsOpacityEffect* effect_out = new QGraphicsOpacityEffect(ui->page);
        ui->page->setGraphicsEffect(effect_out);

        QPropertyAnimation* fade_out = new QPropertyAnimation(effect_out, "opacity");
        fade_out->setDuration(800);
        fade_out->setStartValue(1.0);
        fade_out->setEndValue(0.0);
        fade_out->setEasingCurve(QEasingCurve::InOutQuad);

        connect(fade_out, &QPropertyAnimation::finished, this, [=]() {
            // 화면 전환
            ui->stackedWidget->setCurrentWidget(ui->page_main);

            // 페이드 인 효과
            QGraphicsOpacityEffect* effect_in = new QGraphicsOpacityEffect(ui->page_main);
            ui->page_main->setGraphicsEffect(effect_in);

            QPropertyAnimation* fade_in = new QPropertyAnimation(effect_in, "opacity");
            fade_in->setDuration(800);
            fade_in->setStartValue(0.0);
            fade_in->setEndValue(1.0);
            fade_in->setEasingCurve(QEasingCurve::InOutQuad);
            fade_in->start(QAbstractAnimation::DeleteWhenStopped);

            // 서버 요청은 약간 나중에
            QTimer::singleShot(300, this, [=](){
                QJsonObject req;
                req["signal"] = "search";
                client->sendJson(req);
            });
        });

        fade_out->start(QAbstractAnimation::DeleteWhenStopped);
    });


    //광고 넘어가는 Qtimer
    adTimer = new QTimer(this);
    connect(adTimer, &QTimer::timeout, this, [=](){
        currentAdIndex = (currentAdIndex + 1) % ui->ad_stacked->count();
        ui->ad_stacked->setCurrentIndex(currentAdIndex);
    });
    adTimer->start(1500);

    // 메인화면에서 홈버튼을 누르면 제일 위로 올라감
    connect(ui->BtnHome, &QPushButton::clicked, this, &MainWindow::init_scrollbar);

    // 비회원 로그인 시 MyEats페이지랑 다이얼로그 창 뜨는 걸 분기처리 해야 됨
    connect(ui->BtnMyEats, &QPushButton::clicked, this, &MainWindow::showChoiceLoginDialg);

    // 검색 버튼 누르면 검색 페이지 생성
    connect(ui->BtnUnderSearch, &QPushButton::clicked, this, [=](){
        part3_search *serch = new part3_search(this);
        ui->stackedWidget->addWidget(serch);
        ui->stackedWidget->setCurrentWidget(serch);

        connect(serch, &part3_search::reqTurnToMain, this, [=](){
            ui->stackedWidget->setCurrentWidget(ui->page_main);
        });
    });

    // 주소 버튼 누르면 주소 페이지 생성
    connect(ui->btn_top_addr, &QPushButton::clicked, this, [=](){
        part2_address *addr_page = new part2_address(client, this);
        ui->stackedWidget->addWidget(addr_page);
        ui->stackedWidget->setCurrentWidget(addr_page);

        connect(addr_page, &part2_address::reqTurnToMain, this, [=](){
            ui->stackedWidget->setCurrentWidget(ui->page_main);
        });
    });


    connect(ui->BtnFavorite, &QPushButton::clicked, this, [=](){
        part10_favorite *favorite_page = new part10_favorite(this);
        ui->stackedWidget->addWidget(favorite_page);
        ui->stackedWidget->setCurrentWidget(favorite_page);

        connect(favorite_page, &part10_favorite::reqTurnToMain, this, [=](){
            ui->stackedWidget->setCurrentWidget(ui->page_main);
        });
    });

    // 주문 내역 버튼 -> 리뷰 쓰는 페이지
    connect(ui->BtnOrder, &QPushButton::clicked, this, [=](){
        part9_write_review *write_review = new part9_write_review(client, this);
        ui->stackedWidget->addWidget(write_review);
        ui->stackedWidget->setCurrentWidget(write_review);

        connect(write_review, &part9_write_review::reqTurnToMain, this, [=](){
            ui->stackedWidget->setCurrentWidget(ui->page_main);
        });
    });

    // 주문 흐름 페이지
    connect(&UserState::getInstance(), &UserState::cartUpdated, this, [=](){
        ui->cart_frame->setVisible(true);

        int total_quantity = 0;
        int total_price = 0;
        for (const CartItem& item : UserState::getInstance().cart.items) {
            total_quantity += item.quantity.toInt();
            total_price += item.price.toInt();
        }
        UserState::getInstance().cart.totalPrice = QString::number(total_price);
        QString cart_info = QString::number(total_quantity) + " " + "카트 보기    " + UserState::getInstance().cart.totalPrice + " 원";
        qDebug() << "확인: " << cart_info;
        ui->btn_show_cart_page->setText(cart_info);

        connect(ui->btn_show_cart_page, &QPushButton::clicked, this, [=](){
            part5_cart* cart_page = new part5_cart(client, this);
            ui->stackedWidget->addWidget(cart_page);
            ui->stackedWidget->setCurrentWidget(cart_page);

            connect(cart_page, &part5_cart::reqToMainPage, this, [=](){
                ui->stackedWidget->setCurrentWidget(ui->page_main);
            });

            connect(cart_page, &part5_cart::sucessOrder, this, [=](){
                part6_order_complete* ordered_page = new part6_order_complete(client, this);
                ui->stackedWidget->addWidget(ordered_page);
                ui->stackedWidget->setCurrentWidget(ordered_page);

                connect(ordered_page, &part6_order_complete::reqToMainPage, this, [=](){
                    ui->stackedWidget->setCurrentWidget(ui->page_main);

                });
            });
        });
    });

}

// json 파일 생성
void MainWindow::saveJsonToFile(const QJsonObject& json) {
    // 파일 이름에 타임스탬프를 추가해서 덮어쓰지 않게
    QString fileName = QString("serverresponse%1.json")
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("파일 열기 실패: %s", qPrintable(file.errorString()));
        return;
    }

    QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));  // 보기 좋게 들여쓰기해서 저장
    file.close();

    qDebug() << "JSON 데이터 저장 완료:" << fileName;
}

// 서버로부터 받은 JSON 응답을 여기 처리
void MainWindow::process_json_form_server(){
    connect(client, &TcpClient::jsonReceived, this, [=](QJsonObject json){

        QString signal = json["signal"].toString();
        // Signal 분기 처리 시작
        if (signal == "main_data") {
            QJsonArray jsonData = json["store_info"].toArray();
            this->topData = parseTopData(jsonData);
            this->SettingCard();  // 데이터가 들어온 뒤에 카드 생성
        }
        else if (signal == "keyword_result") {
            qDebug() << json;
        }
        else if (signal == "cart_data") {
            qDebug() << json;
        }
    });

}

// 구조체에 데이터 담기
TopData MainWindow::parseTopData(const QJsonArray& storeInfoArr) {
    TopData topData;

    for (const QJsonValue& catVal : storeInfoArr) {
        QJsonObject catObj = catVal.toObject();
        Category category;
        category.category_name = catObj["category_name"].toString();

        QJsonArray storesArr = catObj["store_info"].toArray();
        for (const QJsonValue& storeVal : storesArr) {
            QJsonObject storeObj = storeVal.toObject();

            StoreInfo store;
            store.store_name = storeObj["store_name"].toString();
            store.category_num = storeObj["category_num"].toInt();
            store.store_addr = storeObj["store_addr"].toString();
            store.phone_num = storeObj["phone_num"].toString();
            store.owner_name = storeObj["owner_name"].toString();
            store.company_num = storeObj["company_number"].toString();
            store.opening_time = storeObj["opening_time"].toString();
            store.closing_time = storeObj["closing_time"].toString();
            store.delivery_fee = storeObj["delivery_fee"].toString();
            store.minimum_order = storeObj["minimum_order"].toString();
            store.store_desc = storeObj["store_desc"].toString();
            store.origin_info = storeObj["origin_info"].toString();
            store.nutrition_fact = storeObj["nutrition_fact"].toString();
            store.allergen_info = storeObj["allergen_info"].toString();
            store.stataus = storeObj["stataus"].toString();

            // 메뉴 파싱
            QJsonObject menuInfoObj = storeObj["menu_info"].toObject();
            QJsonArray menusArr = menuInfoObj["menus"].toArray();
            for (const QJsonValue& menuVal : menusArr) {
                QJsonObject menuObj = menuVal.toObject();
                MenuInfo menu;
                menu.menu_category = menuObj["menu_category"].toString();
                menu.menu_name = menuObj["menu_name"].toString();
                menu.menu_info = menuObj["menu_info"].toString();
                menu.menu_img_path = menuObj["menu_img_path"].toString();
                menu.menu_status = menuObj["menu_status"].toString();
                menu.menu_price = menuObj["menu_price"].toString();

                // menu_options가 비어있거나 문자열일 수 있음
                if (menuObj.contains("menu_options") && menuObj["menu_options"].isArray()) {
                    QJsonArray menuOptionsArr = menuObj["menu_options"].toArray();
                    for (const QJsonValue& optCatVal : menuOptionsArr) {
                        QJsonObject optCatObj = optCatVal.toObject();
                        MenuOption menuOption;
                        menuOption.option_category = optCatObj["option_category"].toString();
                        menuOption.option_required = optCatObj["option_required"].toString();

                        QJsonArray optionsArr = optCatObj["options"].toArray();
                        for (const QJsonValue& optVal : optionsArr) {
                            QJsonObject optObj = optVal.toObject();
                            OptionInfo option;
                            option.option_name = optObj["option_name"].toString();
                            option.option_price = optObj["option_price"].toString();
                            option.option_status = optObj["option_status"].toString();
                            menuOption.options.append(option);
                        }

                        menu.menu_options.append(menuOption);
                    }
                }

                store.menus.append(menu);
            }

            category.restaurants.append(store);
        }

        topData.store_info.append(category);
    }

    return topData;
}

// 세팅 페이지
void MainWindow::setting_main() {
    client = new TcpClient(this);  // MainWindow 소멸 시 같이 삭제됨
    client->connectToServer();     // 연결은 딱 한 번만 하면 됨

    // 메인화면 하단 카트 버튼 비활성화
    ui->cart_frame->setVisible(false);

    // 홈화면에 있으니까 홈 화면만 활성화
    // ui->BtnHome->setStyleSheet("color: black; background-color: white;");

    connect(ui->main_scroll->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int value) {
        qDebug() << "현재 스크롤 위치 : " << value;
    });
    connect(ui->main_scroll->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int value) {
        if(value > 50) show_up_btn();
    });

    // 스크롤 영역에서 마우스 제스처로 옆으로 움직일 수 있도록 설정
    QScroller::grabGesture(ui->scrollArea_side_slide->viewport(), QScroller::LeftMouseButtonGesture);
    // 스크롤 비활성화
    ui->main_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_side_slide->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

// 메인 화면에서 상단 카테고리와 가로 스크롤 카드들을 세팅하는 함수
void MainWindow::SettingCard() {
    // 그리드 형태의 카테고리 위젯 생성 후 레이아웃에 추가
    w_grid_category* grid_category = new w_grid_category;
    ui->layout_category->addWidget(grid_category);

    // 수평 카테고리 리스트 생성 후 스크롤 영역에 세팅
    w_h_category* h_category = new w_h_category;
    ui->h_scroll_category->setWidget(h_category);

    // 카테고리 클릭 시 처리할 슬롯 함수 연결
    connect(grid_category, &w_grid_category::categoryClicked, this, &MainWindow::handleCategoryClicked);

    // 카드들을 담을 컨테이너 위젯과 수평 레이아웃 생성
    QWidget *container = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(container);
    // 카드 간 간격을 좀 해야 예쁨
    layout->setSpacing(20);  // 카드 사이 여백을 더 넓게
    layout->setContentsMargins(10, 0, 10, 0);  // 좌우 여백 추가


    // 카테고리 인덱스 확인용
    int catIndex = 1;
    if (topData.store_info.size() <= catIndex) return;

    QVector<StoreInfo>& stores = topData.store_info[catIndex].restaurants;

    // 전체 카드 개수 제한용
    int count = 0;
    for (int catIndex = 1; catIndex <= 4; ++catIndex) { //0~4번 카테고리까지 순회
        if (topData.store_info.size() <= catIndex) continue;

        QVector<StoreInfo>& stores = topData.store_info[catIndex].restaurants;

        for (const StoreInfo& store : stores) {
            if (count >= 10) break;  // 전체 카드 최대 10개만!
            count++;

            // 가게 카드를 하나 생성하고 크기 설정 -> 좀 작아야 함
            w_v_store_card *store_card = new w_v_store_card;
            store_card->setFixedSize(290, 200);
            // 각 QLabel 객체 이름으로 찾아오기 ~findChild~
            QLabel* nameLabel = store_card->findChild<QLabel*>("store_name");
            QLabel* deliveryLabel = store_card->findChild<QLabel*>("free_delivery");
            QLabel* orderLabel = store_card->findChild<QLabel*>("mini_order");
            QLabel* timeLabel = store_card->findChild<QLabel*>("es_stime");
            QLabel* distanceLabel = store_card->findChild<QLabel*>("distance");
            QLabel* imgLabel = store_card->findChild<QLabel*>("food_picture");
            QLabel* reviewLabel = store_card->findChild<QLabel*>("review");

            // 가게 이름 설정
            if (nameLabel)
                nameLabel->setText(store.store_name);
            if(store.store_name == "KYOCHON") {
                store_card->findChild<QLabel*>("store_name")->setText("교촌치킨 소촌우산점");
            }
            // 배달비 설정
            if (deliveryLabel)
                deliveryLabel->setText("무료배달");
            // 최소 주문 금액 설정
            if (orderLabel)
                orderLabel->setText("최소주문 " + store.minimum_order);
            // 운영 시간 설정
            if (timeLabel)
                timeLabel->setText(store.opening_time + " ~ " + store.closing_time);

            if (distanceLabel) {
                if (store.store_name.contains("KYOCHON")) {
                    distanceLabel->setText("0.2km");
                } else {
                    int randInt = QRandomGenerator::global()->bounded(28);
                    float dist = 0.3f + 0.1f * randInt;
                    distanceLabel->setText(QString::number(dist, 'f', 1) + "km");
                }
            }
            if (reviewLabel) {
                if (store.store_name.contains("KYOCHON")) {
                    reviewLabel->setText("⭐ 4.7");
                } else {
                    int randInt = QRandomGenerator::global()->bounded(9);  // 0~8
                    float score = 4.2f + 0.1f * randInt;
                    reviewLabel->setText("⭐ " + QString::number(score, 'f', 1));
                }
            }



            // 이미지 설정 -> 다른 팀원들은 본인의 해당하는 경로로 수정해야 함!
            if (imgLabel && !store.menus.isEmpty()) {
                QString fileName = store.menus[0].menu_img_path;

                QString path = QDir("/home/y/CouCouEatsUser").filePath(fileName);

                QPixmap pix(path);
                if (!pix.isNull()) {
                    int imgW = 290;
                    int imgH = 130;

                    imgLabel->setFixedSize(imgW, imgH);
                    imgLabel->setPixmap(pix.scaled(imgW, imgH, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
                    imgLabel->setAlignment(Qt::AlignCenter);
                } else {
                    qDebug() << "이미지 로드 실패:" << path;
                }
            }
            // 레이아웃에 카드 추가
            layout->addWidget(store_card);
        }
    }

    // 컨테이너 레이아웃 세팅 후 스크롤 영역에 추가
    container->setLayout(layout);
    ui->scrollArea_side_slide->setWidget(container);
    // 마지막 레이아웃에 카드 세팅!
    addWidgetLastLayout();

}


// 메인 화면에서 마지막 가게 카드 위젯들 세팅하는 함수
void MainWindow::addWidgetLastLayout() {
    int catIndex = 18; // 치킨의 인덱스 값
    if (topData.store_info.size() <= catIndex) return;

    QVector<StoreInfo>& stores = topData.store_info[catIndex].restaurants;

    int count = 0;
    for (const StoreInfo& store : stores) {
        if (count >= 10) break;
        count++;

        w_v_store_card *store_card = new w_v_store_card;
        store_card->setFixedSize(340, 200);

        QLabel* nameLabel = store_card->findChild<QLabel*>("store_name");
        QLabel* deliveryLabel = store_card->findChild<QLabel*>("free_delivery");
        QLabel* orderLabel = store_card->findChild<QLabel*>("mini_order");
        QLabel* timeLabel = store_card->findChild<QLabel*>("es_stime");
        QLabel* distanceLabel = store_card->findChild<QLabel*>("distance");
        QLabel* imgLabel = store_card->findChild<QLabel*>("food_picture");
        QLabel* adLabel = store_card->findChild<QLabel*>("ad_label");
        QLabel* reviewlabel = store_card->findChild<QLabel*>("review");

        if (reviewlabel) {
            int randInt = QRandomGenerator::global()->bounded(0, 9);
            float score = 4.2f + 0.1f * randInt;
            reviewlabel->setText("⭐ " + QString::number(score, 'f', 1));
        }


        if (adLabel)
            adLabel->setFixedWidth(340);

        if (nameLabel)
            nameLabel->setText(store.store_name);

        if (deliveryLabel)
            deliveryLabel->setText("무료배달");

        if (orderLabel)
            orderLabel->setText("최소주문 " + store.minimum_order);

        if (timeLabel)
            timeLabel->setText(store.opening_time + " ~ " + store.closing_time);


        if (distanceLabel) {
            if (store.store_name.contains("교촌")) {
                distanceLabel->setText("0.2km");
            } else {
                int randInt = QRandomGenerator::global()->bounded(28);  // 0~27
                float dist = 0.3f + 0.1f * randInt;
                distanceLabel->setText(QString::number(dist, 'f', 1) + "km");
            }
        }

        if (imgLabel && !store.menus.isEmpty()) {
            QString fileName = store.menus[0].menu_img_path;
            QString path = QDir("/home/y/CouCouEatsUser/").filePath(fileName);

            QPixmap pix(path);
            if (!pix.isNull()) {
                int imgW = 340;
                int imgH = 130;

                imgLabel->setFixedSize(imgW, imgH);
                imgLabel->setPixmap(pix.scaled(imgW, imgH, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
                imgLabel->setAlignment(Qt::AlignCenter);
            } else {
                qDebug() << "❌ 이미지 로드 실패:" << path;
            }
        }

        // layout_last에 직접 붙이기
        ui->layout_last->addWidget(store_card);
    }

}


// 스크롤의 값이 100 이상이 넘어가면 생기는 setValue의 값이 0이 되는 버튼 생성
void MainWindow::show_up_btn() {
    // 올라가는 버튼이 스크롤 영역에 형성된다.
    QPushButton *toTopBtn = new QPushButton(ui->main_scroll);
    toTopBtn->setIcon(QIcon(":/mainIcon/up.png"));
    toTopBtn->setFixedSize(40, 40);
    toTopBtn->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(255, 255, 255, 180);
            border-radius: 20px;
            border: none;
        }
        QPushButton:hover {
            background-color: rgba(200, 200, 200, 180);
        }
    )");
    // 스크롤 영역 위에 표시
    toTopBtn->raise();
    // 우측 하단에 위치 고정
    toTopBtn->move(ui->main_scroll->viewport()->width() - 50, ui->main_scroll->viewport()->height() - 50);
    connect(toTopBtn, &QPushButton::clicked, this, &MainWindow::init_scrollbar);
    toTopBtn->show();
    connect(ui->main_scroll->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int value) {
        if (value == 0) toTopBtn->hide();
    });

}

// 비로그인 시 뜨는 로그인 팝업창
void MainWindow::showChoiceLoginDialg() {

    if(UserState::getInstance().isLoggedIn == false) {
        QWidget *overlay = new QWidget(this);
        overlay->setAttribute(Qt::WA_DeleteOnClose);
        overlay->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        overlay->setAttribute(Qt::WA_TranslucentBackground);
        overlay->resize(this->size());
        overlay->show();

        // 다이얼로그 생성
        QDialog *dialog = new QDialog(overlay);  // parent는 overlay!
        dialog->setWindowFlags(Qt::FramelessWindowHint);
        dialog->setStyleSheet("QDialog { border: 1px solid #999999; background-color: white; }"); //border-radius: 12px;
        // dialog->setFixedSize(340, 290);
        dialog->setFixedSize(380, 310);

        // 수직 레이아웃 구성
        QVBoxLayout *layout = new QVBoxLayout(dialog);

        QLabel *wowLabel = new QLabel("WOW 와우회원은 매 주문 무료배달", dialog);
        wowLabel->setAlignment(Qt::AlignCenter);
        wowLabel->setStyleSheet(R"(
            QLabel {
                background-color: #0039a6;
                color: white;
                font-size: 10pt;
                font-weight: bold;
                padding: 10px;
                border-top-left-radius: 8px;
                border-top-right-radius: 8px;
            }
        )");
        layout->addWidget(wowLabel);

        // 제목
        QLabel *mainLabel = new QLabel("\n쿠팡이츠 로그인", dialog);
        mainLabel->setAlignment(Qt::AlignLeft);
        mainLabel->setStyleSheet("font-size: 15pt; font-weight: bold;");
        layout->addWidget(mainLabel);

        // 부제
        QLabel *subLabel = new QLabel("사용 중인 쿠팡 앱이 있다면\n더 빠르고 간편하게 로그인할 수 있습니다.", dialog);
        subLabel->setAlignment(Qt::AlignLeft);
        subLabel->setStyleSheet("font-size: 11pt;"); //font-weight: bold;
        layout->addWidget(subLabel);

        // 간편 로그인 버튼
        QPushButton *easyLoginBtn = new QPushButton("쿠팡 앱으로 간편 로그인", dialog);
        easyLoginBtn->setMinimumHeight(45);
        easyLoginBtn->setStyleSheet("font-size: 12pt; background-color: #00A8FF; color: white;");
        layout->addWidget(easyLoginBtn);

        // 이메일 로그인 버튼
        QPushButton *emailLoginBtn = new QPushButton("쿠팡 아이디(이메일)로 로그인", dialog);
        emailLoginBtn->setMinimumHeight(45);
        emailLoginBtn->setStyleSheet("font-size: 12pt;");
        layout->addWidget(emailLoginBtn);

        // 가로 정렬 회원가입 안내
        QHBoxLayout *signupLayout = new QHBoxLayout();
        QLabel *signupLabel = new QLabel("아직 회원이 아니신가요?", dialog);
        signupLabel->setStyleSheet("font-size: 10pt;");
        signupLabel->setAlignment(Qt::AlignCenter);
        QPushButton *signupBtn = new QPushButton("회원가입", dialog);
        signupBtn->setFlat(true);
        signupBtn->setCursor(Qt::PointingHandCursor);
        signupBtn->setStyleSheet("font-size: 11pt; color: blue; text-decoration: underline;");
        signupLayout->addWidget(signupLabel);
        signupLayout->addWidget(signupBtn);
        // signupLayout->addStretch(); //오른쪽 여백 없앰
        layout->addLayout(signupLayout);

        //버튼 동작
        QObject::connect(easyLoginBtn, &QPushButton::clicked, dialog, [=]() {
            dialog->close();
            overlay->close();
        });

        //다이얼로그 위치를 하단으로 조정
        int x = (overlay->width() - dialog->width()) / 2;
        int y = (overlay->height() - dialog->height()); //

        dialog->move(x, y);

        //실행
        dialog->show();



        // 회원가입 페이지로 이동
        connect(signupBtn, &QPushButton::clicked, this, [=](){
            login = new part1_login_signup(client, this);
            ui->stackedWidget->addWidget(login);
            ui->stackedWidget->setCurrentWidget(login);

            login->getStackedWidget()->setCurrentWidget(login->getSignupPage());
            dialog->close();
            overlay->close();
            connect(login, &part1_login_signup::reqTurnToMain, this, [=](){
                ui->stackedWidget->setCurrentWidget(ui->page_main);
            });
        });

        // 로그인 페이지로 이동
        connect(emailLoginBtn, &QPushButton::clicked, this, [=](){
            login = new part1_login_signup(client, this);
            ui->stackedWidget->addWidget(login);
            ui->stackedWidget->setCurrentWidget(login);

            login->getStackedWidget()->setCurrentWidget(login->getLoginPage());
            dialog->close();
            overlay->close();

            connect(login, &part1_login_signup::reqTurnToMain, this, [=](){
                ui->stackedWidget->setCurrentWidget(ui->page_main);
            });
        });

    }
    // 마이 이츠 페이지로 이동
    connect(ui->BtnMyEats, &QPushButton::clicked, this, [=](){
        my_page *my_eats_page = new my_page(client, this);
        ui->stackedWidget->addWidget(my_eats_page);
        ui->stackedWidget->setCurrentWidget(my_eats_page);
    });

}

// 카테고리 선택 시 생성되는 카테고리 선택 페이지
void MainWindow::handleCategoryClicked(QString name) {

    part4_clicked_cat_page *cat_page = new part4_clicked_cat_page(name, this->topData, this);
    ui->stackedWidget->addWidget(cat_page);
    ui->stackedWidget->setCurrentWidget(cat_page);


    // 뒤로 가기 연결
    connect(cat_page, &part4_clicked_cat_page::reqToMainPage, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_main);
        cat_page->deleteLater();
    });

    // 카테고리 클릭 연결 (재귀 호출)
    connect(cat_page, &part4_clicked_cat_page::categoryClicked, this, [=](QString newName) {
        cat_page->deleteLater();  // 기존 페이지 삭제

        // "재귀적으로 다시 handleCategoryClicked 실행"
        handleCategoryClicked(newName);
    });

    // 가게 카드 클릭 시 생성되는 가게 메인 페이지
    connect(cat_page, &part4_clicked_cat_page::storeCardClicked, this, [=](QString store_name){
        part7_store_page* store_page = new part7_store_page(store_name, this->topData, client, this);
        ui->stackedWidget->addWidget(store_page);
        ui->stackedWidget->setCurrentWidget(store_page);
        // 가게 페이지에서 뒤로가기 버튼 누를 시 해당 카테고리 페이지로 이동
        connect(store_page, &part7_store_page::reqBackCatPage, this, [=]{
            ui->stackedWidget->setCurrentWidget(cat_page);
        });

        connect(store_page, &part7_store_page::reqMainPage, this, [=]{
            ui->stackedWidget->setCurrentWidget(ui->page_main);
        });
    });
}

// 스크롤바 초기화
void MainWindow::init_scrollbar() {
    ui->main_scroll->verticalScrollBar()->setValue(0);
    ui->h_scroll_category->horizontalScrollBar()->setValue(0);
    ui->scrollArea_side_slide->horizontalScrollBar()->setValue(0);
}

// 검정색 하단 메세지 박스
void MainWindow::showMSG(const QString& message, int ms) {
    QLabel* msg = new QLabel(message, this);  // this는 MainWindow
    msg->setStyleSheet("background-color: rgba(0, 0, 0, 180); color: white; padding: 10px; border-radius: 10px;");
    msg->setAlignment(Qt::AlignCenter);
    msg->setFixedWidth(340);
    msg->setFixedHeight(40);
    msg->move((width() - msg->width())/2, height()-170);
    msg->show();

    QTimer::singleShot(ms, msg, [=]() {
        msg->close();
        msg->deleteLater();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
