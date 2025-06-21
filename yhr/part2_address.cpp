#include "part2_address.h"
#include "ui_part2_address.h"

part2_address::part2_address(TcpClient* client, QWidget *parent)
    : QWidget(parent), ui(new Ui::part2_address), client(client)
{
    ui->setupUi(this);

    settingUI();
    connectedPage();
    showAddrCard();
    registerAddress();

    connect(client, &TcpClient::jsonReceived, this, [=](QJsonObject json) {
        QString signal = json["signal"].toString();

        if (signal == "get_addr_result") {
            UserState::getInstance().addresses.clear();
            QJsonArray arr = json["user_addr"].toArray();
            for (const QJsonValue& val : arr) {
                QJsonObject obj = val.toObject();

                AddressInfo newAddr;
                newAddr.addr_id = obj["addr_id"].toString();
                newAddr.basic = obj["basic_addr"].toString();
                newAddr.detail = obj["detail_addr"].toString();
                newAddr.direction = obj["direction"].toString();
                newAddr.type = obj["type"].toString();
                newAddr.isPrimary = obj["is_primary"].toString() == "1";

                UserState::getInstance().addAddress(newAddr);
            }
            showAddrCard();
            ui->stackedWidget->setCurrentWidget(ui->addr_main_page);
        }
        qDebug() << json;

    });
}



void part2_address::showAddrCard() {
    QWidget* container = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 0, 0);

    QString selectedType;
    if (ui->btnAddHome->isChecked()) selectedType = "집";
    else if (ui->btnAddCompany->isChecked()) selectedType = "회사";

    const auto& addresses = UserState::getInstance().addresses;
    for (int i = 0; i < addresses.size(); ++i) {
        const AddressInfo& addr = addresses[i];

        if (!selectedType.isEmpty() && addr.type != selectedType)
            continue;

        w_v_addr_card* addr_card = new w_v_addr_card(this);
        addr_card->findChild<QLabel*>("label_basic")->setText(addr.basic);
        addr_card->findChild<QLabel*>("label_detail")->setText(addr.detail);


        QPushButton* btn_detail = addr_card->findChild<QPushButton*>("btn_edit_addr");
        if (btn_detail) {
            btn_detail->setProperty("index", i);
            connect(btn_detail, &QPushButton::clicked, this, [=]() {
                int index = btn_detail->property("index").toInt();
                if (index < 0 || index >= UserState::getInstance().addresses.size()) return;

                const AddressInfo& selected = UserState::getInstance().addresses[index];
                UserState::getInstance().selectedAddressIndex = index;

                ui->label_top->setText(selected.basic);
                ui->label_basic->setText(selected.detail);
                ui->line_detail->setText(selected.detail);
                ui->line_direction->setText(selected.direction);

                ui->btn_home->setChecked(selected.type == "집");
                ui->btn_company->setChecked(selected.type == "회사");
                ui->btn_else->setChecked(selected.type != "집" && selected.type != "회사");

                disconnect(ui->btn_delete, nullptr, nullptr, nullptr);
                connect(ui->btn_delete, &QPushButton::clicked, this, [=]() {
                    if (QMessageBox::question(this, "주소 삭제", "정말 삭제하시겠습니까?",
                                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                        QJsonObject req;
                        req["signal"] = "del_addr";
                        req["role"] = "user";
                        req["email"] = UserState::getInstance().email;
                        req["addr_id"] = addr.addr_id;
                        client->sendJson(req);

                        qDebug() << "주소 ID: " << addr.addr_id;
                        qDebug() << "주소 삭제 갔다!!";

                        if (index >= 0 && index < UserState::getInstance().addresses.size()) {
                            UserState::getInstance().addresses.removeAt(index);
                        }
                        showAddrCard();
                        ui->stackedWidget->setCurrentWidget(ui->addr_main_page);
                    }
                });

                ui->btn_delete->show();
                ui->stackedWidget->setCurrentWidget(ui->detail_addr_page);
            });
        }

        layout->addWidget(addr_card);
    }

    layout->addSpacerItem(new QSpacerItem(5, 5, QSizePolicy::Expanding, QSizePolicy::Expanding));
    container->setLayout(layout);
    ui->scrollArea_addr->setWidget(container);
}

void part2_address::registerAddress() {
    QButtonGroup* btnGroup = new QButtonGroup(this);
    btnGroup->setExclusive(true);
    btnGroup->addButton(ui->btn_home);
    btnGroup->addButton(ui->btn_company);
    btnGroup->addButton(ui->btn_else);

    ui->btn_home->setCheckable(true);
    ui->btn_company->setCheckable(true);
    ui->btn_else->setCheckable(true);

    QString checkedStyle = R"(
        QPushButton:checked {
            background-color: #0078D7;
            color: white;
            font-weight: bold;
        }
    )";
    ui->btn_home->setStyleSheet(checkedStyle);
    ui->btn_company->setStyleSheet(checkedStyle);
    ui->btn_else->setStyleSheet(checkedStyle);

    connect(ui->btnConfirm, &QPushButton::clicked, this, [=]() {
        QAbstractButton* selected = btnGroup->checkedButton();
        QString selectedType = selected ? selected->text() : "";

        QJsonObject req;
        req["email"] = UserState::getInstance().email;
        req["basic"] = ui->label_basic->text();
        req["detail"] = ui->line_detail->text();
        req["direction"] = ui->line_direction->text();
        req["type"] = selectedType;

        if (ui->stackedWidget->currentWidget() == ui->detail_addr_page) {
            req["signal"] = "register_addr";
            req["role"] = "user";
            req["is_primary"] = UserState::getInstance().addresses.isEmpty() ? "1" : "0";
        }

        client->sendJson(req);

        ui->line_detail->clear();
        ui->line_direction->clear();
    });
}

void part2_address::connectedPage() {
    connect(ui->btnNowLocation, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->find_now_location_page);
    });
    connect(ui->btnAddHome, &QPushButton::clicked, this, [=](){
        ui->btn_home->setChecked(false);
        ui->btn_company->setChecked(false);
        ui->btn_else->setChecked(false);
        ui->stackedWidget->setCurrentWidget(ui->add_addr_page);
        ui->label_type->setText("집 주소 선택");
        showAddrCard();
    });
    connect(ui->btnAddCompany, &QPushButton::clicked, this, [=](){
        ui->btn_home->setChecked(false);
        ui->btn_company->setChecked(false);
        ui->btn_else->setChecked(false);
        ui->stackedWidget->setCurrentWidget(ui->add_addr_page);
        ui->label_type->setText("회사 주소 선택");
        showAddrCard();
    });
    connect(ui->btnSetting, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->detail_addr_page);
        ui->label_top->setText(ui->label_setting_top->text());
        ui->label_basic->setText(ui->label_setting_bottom->text());
        ui->btn_delete->hide();
    });

    connect(ui->btnBackToMain, &QPushButton::clicked, this, [=](){emit reqTurnToMain();});
    connect(ui->btnBack, &QPushButton::clicked, this, [=](){ui->stackedWidget->setCurrentWidget(ui->addr_main_page);});
    connect(ui->btn_BackToInit, &QPushButton::clicked, this, [=](){ui->stackedWidget->setCurrentWidget(ui->addr_main_page);});
    connect(ui->btn_find_now_page, &QPushButton::clicked, this, [=](){ui->stackedWidget->setCurrentWidget(ui->find_now_location_page);});
}

void part2_address::settingUI() {

    ui->line_detail->setPlaceholderText("상세주소(아파트/동/호)");
    ui->line_direction->setPlaceholderText("길 안내(예: 1층에 올리브영이 있는 오피스텔)");
    ui->lineEdit->setPlaceholderText("도로명, 건물명 또는 지번으로 검색");
    ui->lineEdit_2->setPlaceholderText("도로명, 건물명 또는 지번으로 검색");

    ui->btn_delete->hide();

    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScroller::grabGesture(ui->scrollArea_2->viewport(), QScroller::LeftMouseButtonGesture);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QTimer::singleShot(1000, this, [=]() {
        QScrollBar* vScrollBar = ui->scrollArea->verticalScrollBar();
        int midValue = (vScrollBar->maximum() + vScrollBar->minimum()) / 2;
        vScrollBar->setValue(midValue);
    });
}

part2_address::~part2_address() {
    delete ui;
}

