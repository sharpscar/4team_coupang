#include "part5_cart.h"
#include "ui_part5_cart.h"

part5_cart::part5_cart(TcpClient* client, QWidget *parent)
    : QWidget(parent), ui(new Ui::part5_cart), client(client)
{
    ui->setupUi(this);

    ui->radio_free->setStyleSheet("border:none;");
    ui->radio_paid->setStyleSheet("border:none;");
    // 이벤트 필터 등록
    ui->frame_free->installEventFilter(this);
    ui->frame_paid->installEventFilter(this);

    // 은행 리스트 콤보박스 추가
    QStringList bankList = {"농협은행", "카카오뱅크", "신한은행", "국민은행", "하나은행"};
    ui->for_payment1->addItems(bankList);
    ui->for_payment2->addItems(bankList);
    ui->for_payment3->addItems(bankList);
    ui->for_payment1_3->addItems(bankList);
    ui->for_payment2_3->addItems(bankList);
    ui->for_payment3_3->addItems(bankList);

    // 요청사항 콤보박스
    QStringList request = {
        "직접 받을게요(부재시 문 앞)",
        "문 앞에 놔주세요(초인종 O)",
        "문 앞에 놔주세요(초인종 X)",
        "도착하면 전화해주세요",
        "도착하면 문자해주세요"
    };
    ui->request_for_dil->addItems(request);

    // 스타일 설정
    ui->x_btn->setStyleSheet("border: none;");
    ui->x_btn_3->setStyleSheet("border: none;");
    ui->free_money->setStyleSheet("border:free; color: red;");
    ui->add_money->setStyleSheet("border:free;");
    ui->what_time->setStyleSheet("border:free;");
    ui->what_time2->setStyleSheet("border:free;");
    //버튼 테두리 지우고 + 둥글게
    ui->btndelivery->setStyleSheet("border: none;border-radius: 50px;");
    ui->btnpickup->setStyleSheet("border: none; border-radius: 50px;");
    ui->btndelivery_2->setStyleSheet("border: none;border-radius: 50px;");
    ui->btnpickup_2->setStyleSheet("border: none;border-radius: 50px;");
    // 배달/포장 버튼 스타일
    ui->btndelivery->setCheckable(true);
    ui->btnpickup->setCheckable(true);
    ui->btndelivery_2->setCheckable(true);
    ui->btnpickup_2->setCheckable(true);

    QString onStyle = "QPushButton { background-color: #212529; color: white; border: none; border-radius: 20px; padding: 6px 20px; }";
    QString offStyle = "QPushButton { background-color: transparent; color: #212529; border: none; border-radius: 20px; padding: 6px 20px; }";

    // ui->label_addr->setText()
    // 초기 상태: 배달
    ui->btndelivery->setStyleSheet(onStyle);
    ui->btnpickup->setStyleSheet(offStyle);
    ui->btndelivery_2->setStyleSheet(onStyle);
    ui->btnpickup_2->setStyleSheet(offStyle);
    ui->stackedWidget->setCurrentIndex(0);

    // 배달 버튼 클릭 시
    connect(ui->btndelivery, &QPushButton::clicked, this, [=]() {
        ui->btndelivery->setStyleSheet(onStyle);
        ui->btnpickup->setStyleSheet(offStyle);
        ui->btndelivery_2->setStyleSheet(onStyle);
        ui->btnpickup_2->setStyleSheet(offStyle);
        ui->btndelivery->setChecked(true);
        ui->btnpickup->setChecked(false);
        ui->btndelivery_2->setChecked(true);
        ui->btnpickup_2->setChecked(false);
        ui->stackedWidget->setCurrentIndex(0);
    });

    // 포장 버튼 클릭 시 (아래쪽)
    connect(ui->btnpickup_2, &QPushButton::clicked, this, [=]() {
        ui->btndelivery->setStyleSheet(offStyle);
        ui->btnpickup->setStyleSheet(onStyle);
        ui->btndelivery_2->setStyleSheet(offStyle);
        ui->btnpickup_2->setStyleSheet(onStyle);
        ui->btndelivery->setChecked(false);
        ui->btnpickup->setChecked(true);
        ui->btndelivery_2->setChecked(false);
        ui->btnpickup_2->setChecked(true);
        ui->stackedWidget->setCurrentIndex(1);
    });

    // 위쪽 포장 버튼 (양방향 동기화)
    connect(ui->btnpickup, &QPushButton::clicked, ui->btnpickup_2, &QPushButton::click);
    connect(ui->btndelivery_2, &QPushButton::clicked, ui->btndelivery, &QPushButton::click);

    // 기타 버튼 스타일
    ui->modify_btn->setStyleSheet("border: none; color: #0078FF;");
    ui->add_menu_btn->setStyleSheet(
        "QPushButton {"
        "  border: 2px solid #0078FF;"
        "  color: #0078FF;"
        "  border-radius: 10px;"
        "  background-color: white;"
        "  padding: 6px 12px;"
        "}"
        );

    // 배송옵션 라디오 버튼 그룹 설정
    deliveryGroup = new QButtonGroup(this);
    deliveryGroup->addButton(ui->radio_free);
    deliveryGroup->addButton(ui->radio_paid);
    deliveryGroup->setExclusive(true);

    connect(deliveryGroup, &QButtonGroup::buttonToggled,
            this, &part5_cart::onDeliveryOptionToggled);

    // 기본 옵션 설정
    selectDeliveryOption(0);
    ui->radio_free->setChecked(true);

    connect(ui->x_btn, &QPushButton::clicked, this, [=](){
        emit reqToMainPage();
    });

    connect(ui->x_btn_3, &QPushButton::clicked, this, [=](){
        emit reqToMainPage();
    });

    // 주문하기 버튼 클릭 시
    connect(ui->payment_btn_2, &QPushButton::clicked ,this, &part5_cart::sendOrderToserver);
    connect(ui->payment_btn_3, &QPushButton::clicked ,this, &part5_cart::sendOrderToserver);

    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
    // 스크롤 비활성화
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

// 서버에게 주문 데이터 보내기
void part5_cart::sendOrderToserver() {

    //결제하기 테스트
    QJsonObject req;
    req["signal"] = "to_pay";
    req["role"] = "user";
    req["user_pnum"] = UserState::getInstance().phoneNumber;
    req["email"] = UserState::getInstance().email;
    req["once_plastic"] = ui->checkBox_2->isChecked() ? "yes" : "no";

    this->fullAddress;
    for (const AddressInfo& addr : UserState::getInstance().addresses) {
        if (addr.isPrimary) {
            this->fullAddress = addr.basic + " " + addr.detail;
            break;
        }
    }
    req["user_addr"] = this->fullAddress;
    req["to_owner"] = ui->lineEdit->text();
    req["to_rider"] = ui->request_for_dil->currentText();
    req["store_name"] = UserState::getInstance().cart.storeName;
    req["total_price"] = UserState::getInstance().cart.totalPrice;
    req["eat_type"] = UserState::getInstance().cart.eatType;
    req["business_num"] = "11111111111";
    QJsonArray menuArray;

    // 장바구니 데이터
    for (const CartItem& item : UserState::getInstance().cart.items) {
        QJsonObject menuObj;
        menuObj["menu_name"] = item.menuName;
        menuObj["quantity"] = item.quantity;
        menuObj["option"] = item.option;
        menuObj["price"] = item.price;
        menuArray.append(menuObj);
    }
    req["menus"] = menuArray;

    qDebug() << "json 확인: " << req;
    // 전송
    client->sendJson(req);

    // 주문 완료 후 주문 확인 페이지로 이동을 위한 시그널
    emit sucessOrder();
    // 장바구니를 비움과 동시에 emit orderComplete
    UserState::getInstance().clearCart();

}

void part5_cart::onDeliveryOptionToggled(QAbstractButton* button, bool checked)
{
    if (!checked) return;

    if (button == ui->radio_free) {
        ui->frame_free->setStyleSheet("border: 2px solid #0078FF; background-color: white; border-radius: 12px;");
        ui->frame_paid->setStyleSheet("background-color: #F5F5F5; border: 1px solid #CCCCCC; border-radius: 12px;");
    } else if (button == ui->radio_paid) {
        ui->frame_paid->setStyleSheet("border: 2px solid #0078FF; background-color: white; border-radius: 12px;");
        ui->frame_free->setStyleSheet("background-color: #F5F5F5; border: 1px solid #CCCCCC; border-radius: 12px;");
    }
}

void part5_cart::selectDeliveryOption(int index) {
    if (index == 0) {
        ui->frame_free->setStyleSheet("background-color: white; border: 2px solid #0078FF; border-radius: 12px;");
    } else {
        ui->frame_paid->setStyleSheet("background-color: white; border: 2px solid #0078FF; border-radius: 12px;");
    }
}

bool part5_cart::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (watched == ui->frame_free) {
            ui->radio_free->setChecked(true);
            return true;
        } else if (watched == ui->frame_paid) {
            ui->radio_paid->setChecked(true);
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

part5_cart::~part5_cart()
{
    delete ui;
}
