#include "part1_login_signup.h"
#include "ui_part1_login_signup.h"

part1_login_signup::part1_login_signup(TcpClient *client, QWidget *parent)
    : QWidget(parent), ui(new Ui::part1_login_signup), client(client)
{
    ui->setupUi(this);

    connect(client, &TcpClient::jsonReceived, this, [=](QJsonObject json){

    // 스크롤 영역에서 마우스 제스처로 옆으로 움직일 수 있도록 설정
    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
    // 스크롤 비활성화
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


        QString signal = json["signal"].toString();
        if (signal == "sign_up_result") {
            QString message = json["result"].toString();
            if(message == "success"){
                QMessageBox::information(this, "입력 완료", "입력이 확정되어 더 이상 수정할 수 없습니다.");
                ui->input_email->setEnabled(false);
                ui->input_pass->setEnabled(false);
                ui->input_name->setEnabled(false);
                ui->input_phone->setEnabled(false);
                ui->birthday->setEnabled(false);
            } else if (message == "fail"){
                qDebug() << "여기 Fail";
                QMessageBox::warning(this, "알림", "※ 이미 등록된 번호는 사용할 수 없습니다 ※");
                ui->input_phone->clear();
            }
        }
        else if (signal == "login_result") {

            QString message = json["result"].toString();
            if(message == "success"){
                // user_info 객체 먼저 꺼내기
                QJsonObject userInfo = json["user_info"].toObject();

                // 로그인 정보 저장
                UserState::getInstance().login(
                    userInfo["name"].toString(),
                    userInfo["email"].toString(),
                    userInfo["pnum"].toString()
                    );

                // 유저 로그인 상태로 변경
                UserState::getInstance().isLoggedIn = true;

                // 카트 정보 저장 (배열로 변경됨!)
                QJsonArray cartArr = userInfo["cart"].toArray();
                if (!cartArr.isEmpty()) {
                    QJsonObject cart_info = cartArr[0].toObject();  // 첫 번째 카트

                    CartInfo cart;
                    cart.storeName = cart_info["store_name"].toString();
                    cart.eatType = cart_info["eat_type"].toString();
                    cart.totalPrice = cart_info["total_price"].toString();  // toInt()

                    QJsonArray items_info = cart_info["cart_items"].toArray();
                    for (const QJsonValue& item : items_info) {
                        CartItem items;
                        QJsonObject item_obj  = item.toObject();
                        items.menuName = item_obj["MENU"].toString();
                        items.option = item_obj["OPTION"].toString();
                        items.price = item_obj["PRICE"].toString();
                        items.quantity = item_obj["QUANTITY"].toString();
                        cart.items.append(items);
                    }

                    UserState::getInstance().cart = cart;
                    // cart 업데이트 시그널 보내기
                    emit UserState::getInstance().cartUpdated();
                }

                // 주소 정보 저장
                QJsonArray addrArr = userInfo["user_add"].toArray();
                for (const QJsonValue& val : addrArr) {
                    QJsonObject addrObj = val.toObject();
                    AddressInfo addr;
                    addr.addr_id = addrObj["addr_id"].toString();
                    addr.basic = addrObj["basic_addr"].toString();
                    addr.detail = addrObj["detail_addr"].toString();
                    addr.direction = addrObj["direction"].toString();
                    addr.type = addrObj["type"].toString();
                    addr.isPrimary = addrObj["is_primary"].toString() == "1";
                    UserState::getInstance().addAddress(addr);
                }

                qDebug() << "주소의 사이즈 확인: "  << UserState::getInstance().addresses.size();

                // 주소 변경됐다는 시그널을 보내기
                emit UserState::getInstance().addressChanged();  // 필요 시

            } else if (message == "fail"){
                QMessageBox::warning(this, "알림", "※ 이메일 또는 비밀번호가 일치하지 않습니다. ※");
                ui->insert_id->clear();
                ui->insert_pass->clear();
            }
        }
    });

    connect(ui->makeid_btn, &QPushButton::clicked, this, &part1_login_signup::move_page);
    make_id();
    connect(ui->fix_btn, &QPushButton::clicked, this, &part1_login_signup::fix_everything);

    // connect(ui->back_to_login, &QPushButton::clicked, this, &part1_login_signup::move_page_to_login);
    connect(ui->back_to_login_2, &QPushButton::clicked, this, &part1_login_signup::move_page_to_login);
    connect(ui->back_to_login_3, &QPushButton::clicked, this, &part1_login_signup::move_page_to_login);

    connect(ui->search_id, &QPushButton::clicked, this, &part1_login_signup::move_page_to_search_id);
    connect(ui->for_search_id, &QPushButton::clicked, this, &part1_login_signup::move_page_to_search_id);
    connect(ui->for_search_id1, &QPushButton::clicked, this, &part1_login_signup::move_page_to_search_id);

    connect(ui->search_pw, &QPushButton::clicked, this, &part1_login_signup::move_page_to_search_pw);
    connect(ui->for_search_pw, &QPushButton::clicked, this, &part1_login_signup::move_page_to_search_pw);
    connect(ui->for_search_pw1, &QPushButton::clicked, this, &part1_login_signup::move_page_to_search_pw);
    connect(ui->insert_pass, &QLineEdit::returnPressed, this, [=](){
        QJsonObject req;
        req["signal"] = "login";
        req["e_mail"] = ui->insert_id->text();
        req["pw"] = ui->insert_pass->text();
        req["role"] = "user";
        client->sendJson(req);
    });

    ui->search_id->setStyleSheet("QPushButton { border: none; color : rgb(17, 62, 245)};");
    ui->search_pw->setStyleSheet("QPushButton { border: none; color : rgb(17, 62, 245)};");
    ui->search_id1->setStyleSheet("border: none;");
    ui->search_id2->setStyleSheet("border : none;");
    ui->search_id1->setPlaceholderText("이름을 입력해주세요");
    ui->search_id2->setPlaceholderText("휴대폰 번호를 입력해주세요");

    ui->search_pw1->setStyleSheet("border: none;");
    ui->search_pw2->setStyleSheet("border : none;");
    ui->search_pw3->setStyleSheet("border : none;");
    ui->search_pw1->setPlaceholderText("이름을 입력해주세요");
    ui->search_pw2->setPlaceholderText("휴대폰 번호를 입력해주세요");
    ui->search_pw3->setPlaceholderText("이메일을 입력해주세요");

    connect(ui->login_btn, &QPushButton::clicked, this, [=](){
        QJsonObject req;
        req["signal"] = "login";
        req["e_mail"] = ui->insert_id->text();
        req["pw"] = ui->insert_pass->text();
        req["role"] = "user";
        client->sendJson(req);
    });

    // connect(ui->back_to_main, &QPushButton::clicked, this, [=](){
    //     emit reqTurnToMain();
    // });

    QList<QCheckBox*> checkList = {
        ui->must_1, ui->must_2, ui->must_3, ui->must_4, ui->must_5
    };
    QList<QCheckBox*> optionalCheckList = {
        ui->select_1, ui->select_2, ui->select_3, ui->select_4, ui->select_5, ui->select_6
    };
    QList<QCheckBox*> allCheckList = checkList + optionalCheckList;

    for (QCheckBox* cb : allCheckList) {
        connect(cb, &QCheckBox::toggled, this, [=]() {
            bool allChecked = std::all_of(allCheckList.begin(), allCheckList.end(),
                                          [](QCheckBox* c) { return c->isChecked(); });
            ui->check_all->blockSignals(true);
            ui->check_all->setChecked(allChecked);
            ui->check_all->blockSignals(false);
        });
    }

    connect(ui->check_all, &QCheckBox::toggled, this, [=](bool checked) {
        for (QCheckBox* cb : checkList + optionalCheckList) {
            cb->setChecked(checked);
        }
    });

    for (QCheckBox* cb : checkList) {
        connect(cb, &QCheckBox::toggled, this, &part1_login_signup::updateSignupButton);
    }
}

void part1_login_signup::make_id() {
    ui->signup_btn->setEnabled(false);
    ui->fix_btn->setEnabled(false);
    ui->login_btn->setStyleSheet("QPushButton { background-color : rgb(17, 62, 245); color : white; border-radius: 7px; }");
    ui->insert_id->setStyleSheet("border: none;");
    ui->insert_pass->setStyleSheet("border : none;");

    ui->input_email->setPlaceholderText("이메일을 입력해주세요");
    connect(ui->input_email, &QLineEdit::textChanged, this, [=](const QString &text) {
        static bool email_valid = false;
        QRegularExpression regex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
        bool now_valid = regex.match(text).hasMatch();
        if (now_valid != email_valid) {
            check_count += (now_valid ? 1 : -1);
            email_valid = now_valid;
            updateSignupButton();
        }
        ui->email_check_icon->setText(now_valid ? "✔" : "");
        ui->email_check_icon->setStyleSheet("color: green;");
    });

    ui->input_pass->setPlaceholderText("8~11자 비밀번호를 입력해주세요");
    ui->input_pass->setEchoMode(QLineEdit::Password);
    connect(ui->input_pass, &QLineEdit::textChanged, this, [=](const QString &text) {
        static bool pass_valid = false;
        QRegularExpression regex("^[A-Za-z0-9]{8,11}$");
        bool now_valid = regex.match(text).hasMatch();
        if (now_valid != pass_valid) {
            check_count += (now_valid ? 1 : -1);
            pass_valid = now_valid;
            updateSignupButton();
        }
        ui->pass_check_icon->setText(now_valid ? "✔" : "");
        ui->pass_check_icon->setStyleSheet("color: green;");
        ui->pass_notice->setText(now_valid ? "사용 가능한 비밀번호 입니다." : "");
        ui->pass_notice->setStyleSheet("color: green;");
    });

    ui->input_name->setPlaceholderText("이름을 입력해주세요");
    connect(ui->input_name, &QLineEdit::textChanged, this, [=](const QString &text) {
        static bool name_valid = false;
        bool now_valid = !text.trimmed().isEmpty();
        if (now_valid != name_valid) {
            check_count += (now_valid ? 1 : -1);
            name_valid = now_valid;
            updateSignupButton();
        }
        ui->name_check_icon->setText(now_valid ? "✔" : "");
        ui->name_check_icon->setStyleSheet("color: green;");
    });

    ui->input_phone->setPlaceholderText("전화번호를 입력해주세요");
    connect(ui->input_phone, &QLineEdit::textChanged, this, [=](const QString &text) {
        static bool phone_valid = false;
        QRegularExpression regex("^[0-9]{11}$");
        bool now_valid = regex.match(text).hasMatch();
        if (now_valid != phone_valid) {
            check_count += (now_valid ? 1 : -1);
            phone_valid = now_valid;
            updateSignupButton();
        }
        ui->num_check_icon->setText(now_valid ? "✔" : "");
        ui->num_check_icon->setStyleSheet("color: green;");
    });

    ui->birthday->setCalendarPopup(true);
    ui->birthday->setDisplayFormat("yyyy-MM-dd");
    ui->birthday->setDate(QDate(2000, 1, 1));
    ui->birthday->setMaximumDate(QDate::currentDate().addYears(-14));
    connect(ui->birthday, &QDateEdit::dateChanged, this, [=](const QDate &date) {
        static bool birth_valid = false;
        bool now_valid = (date <= QDate::currentDate().addYears(-14));
        if (now_valid != birth_valid) {
            check_count += (now_valid ? 1 : -1);
            birth_valid = now_valid;
            updateSignupButton();
            if (check_count == 5)
                ui->fix_btn->setEnabled(true);
        }
    });

    connect(ui->signup_btn, &QPushButton::clicked, this, [=](){
        QJsonObject req;
        req["signal"] = "sign_up";
        req["e_mail"] = ui->input_email->text();
        req["pw"] = ui->input_pass->text();
        req["name"] = ui->input_name->text();
        req["pnum"] = ui->input_phone->text();
        req["birth"] = ui->birthday->date().toString("yyyy-MM-dd");
        req["role"] = "user";
        client->sendJson(req);

        // 회원가입 완료 후 호출
        QMessageBox::information(this, "회원가입 완료", "회원가입이 성공적으로 완료되었습니다!");
        ui->stackedWidget->setCurrentWidget(ui->login_page);
    });
}

void part1_login_signup::fix_everything() {
    QJsonObject req;
    req["signal"] = "sign_up_result";
    req["pnum"] = ui->input_phone->text();
    req["role"] = "user";
    client->sendJson(req);
}

QStackedWidget* part1_login_signup::getStackedWidget() const {
    return ui->stackedWidget;
}

QWidget* part1_login_signup::getSignupPage() const {
    return ui->signup_page;
}

QWidget* part1_login_signup::getLoginPage() const {
    return ui->login_page;
}

void part1_login_signup::updateSignupButton() {
    bool allChecked =
        ui->must_1->isChecked() &&
        ui->must_2->isChecked() &&
        ui->must_3->isChecked() &&
        ui->must_4->isChecked() &&
        ui->must_5->isChecked();
    ui->signup_btn->setEnabled(check_count == 5 && allChecked);
}

void part1_login_signup::move_page() {
    ui->stackedWidget->setCurrentIndex(1);
}

void part1_login_signup::move_page_to_login() {
    ui->stackedWidget->setCurrentIndex(0);
}

void part1_login_signup::move_page_to_search_id() {
    ui->stackedWidget->setCurrentIndex(2);
}

void part1_login_signup::move_page_to_search_pw() {
    ui->stackedWidget->setCurrentIndex(3);
}

part1_login_signup::~part1_login_signup() {
    delete ui;
}

