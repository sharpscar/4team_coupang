#include "part8_menu_option_page.h"
#include "ui_part8_menu_option_page.h"

part8_menu_option_page::part8_menu_option_page(TcpClient* client, QString menu_name, TopData& data, QWidget *parent)
    : QWidget(parent), client(client), menu_name(menu_name), ui(new Ui::part8_menu_option_page), data(data)
{
    ui->setupUi(this);

    settingUI();
}



void part8_menu_option_page::settingUI() {

    ui->stickyMenuBar->setVisible(false);
    // 이전 가게 페이지로 이동
    connect(ui->btnBackToStorePage, &QPushButton::clicked, this, [=](){
        emit reqBackStorePage();
    });

    // 메뉴 찾기
    this->currentMenu = findMenuInTopData(menu_name);
    if (!this->currentMenu) return;

    QString relativePath = currentMenu->menu_img_path;

    if (relativePath.startsWith("./"))
        relativePath = relativePath.mid(2);
    if (relativePath.startsWith("KYOCHON_IMG/"))
        relativePath = ":/KYOCHON_IMG/" + relativePath.mid(QString("KYOCHON_IMG/").length());


    QPixmap pix(relativePath);
    if (!pix.isNull()) {
        int fixedWidth = 350;
        int fixedHeight  = 170;
        QPixmap scaled = pix.scaledToWidth(fixedWidth, Qt::SmoothTransformation);
        ui->label_menu_img->setPixmap(scaled);
        ui->label_menu_img->setFixedWidth(fixedWidth);
        ui->label_menu_img->setFixedHeight(fixedHeight);
        ui->label_menu_img->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    } else {
        qDebug() << "리소스 이미지 로딩 실패!";
        ui->label_menu_img->setText("이미지 없음");
    }

    // 메뉴 정보 표시
    // ui->label_menu_img.
    ui->label_menu_name_2->setText(this->currentMenu->menu_name);
    ui->label_menu_info->setText(this->currentMenu->menu_info);
    ui->label_menu_price->setText(this->currentMenu->menu_price + "원");

    // 옵션 UI 표시
    clearLayout(ui->layout_option);
    drawMenuOptions();

    // + 버튼 누를 시 수량 증가
    connect(ui->btn_add, &QPushButton::clicked, this, [=](){
        int count = ui->label_menu_count->text().toInt();
        count += 1;
        ui->label_menu_count->setText(QString::number(count));
        int price = ui->label_menu_price->text().toInt();
        ui->label_menu_price->setText(QString::number(price*count));
    });
    // - 버튼 누를 수 수량 감소
    connect(ui->btn_abstract, &QPushButton::clicked, this, [=](){
        int count = ui->label_menu_count->text().toInt();
        count -= 1;
        ui->label_menu_count->setText(QString::number(count));
        int price = ui->label_menu_price->text().toInt();
        ui->label_menu_price->setText(QString::number(price*count));
    });

    connect(ui->btnInputOrder, &QPushButton::clicked, this, [=]() {
        if (!validateRequiredOptions()) {
            qDebug() << "필수 옵션 선택 안됨";
            return;
        }

        QStringList selectedOptions;
        int additionalOptionPrice = 0;

        for (int i = 0; i < ui->layout_option->count(); ++i) {
            QFrame* frame = qobject_cast<QFrame*>(ui->layout_option->itemAt(i)->widget());
            if (!frame) continue;
            QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(frame->layout());
            for (int j = 0; j < layout->count(); ++j) {
                QAbstractButton* btn = qobject_cast<QAbstractButton*>(layout->itemAt(j)->widget());
                if (btn && btn->isChecked()) {
                    QString optionText = btn->text();  //
                    selectedOptions.append(optionText);

                    // 옵션 가격 추출
                    QRegularExpression re("\\+([0-9]+)원");
                    QRegularExpressionMatch match = re.match(optionText);
                    if (match.hasMatch()) {
                        additionalOptionPrice += match.captured(1).toInt();  // 500
                    }
                }
            }
        }

        // 수량/금액 계산
        int count = ui->label_menu_count->text().toInt();
        int basePrice = currentMenu->menu_price.toInt();
        // 메뉴 기본 가격
        int singleItemPrice = basePrice + additionalOptionPrice;
        int totalPrice = singleItemPrice * count;

        // CartItem 구성
        CartItem item;
        item.menuName = menu_name;
        item.quantity = QString::number(count);
        item.option = selectedOptions.join(", ");
        item.price = QString::number(totalPrice);

        UserState::getInstance().cart.items.append(item);

        // 장바구니 JSON 구성
        QJsonObject req;
        req["signal"] = "save_cart";
        req["role"] = "user";
        req["email"] = UserState::getInstance().email;
        req["store_name"] = UserState::getInstance().cart.storeName;
        req["status"] = UserState::getInstance().cart.eatType;

        int totalCartPrice = 0;
        QJsonArray menusArray;
        for (const CartItem& ci : UserState::getInstance().cart.items) {
            QJsonObject menuObj;
            menuObj["menu_name"] = ci.menuName;
            menuObj["quantity"] = ci.quantity;
            menuObj["option"] = ci.option;
            menuObj["price"] = ci.price;
            menusArray.append(menuObj);

            totalCartPrice += ci.price.toInt();
        }
        req["total_price"] = QString::number(totalCartPrice);
        req["menus"] = menusArray;

        // 서버에게 보내는 save_cart 정보

        client->sendJson(req);

        UserState::getInstance().cartUpdated();
    });

    connect(ui->scrollArea_option->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int value){
        // sticky가 기준이 되는 위치
        int stickyY = ui->label_sticky->y();

        // 현재 스크롤 위치에 따라 sticky 메뉴바를 보이거나 숨김
        if (value >= stickyY) {
            // 메뉴바 고정
            ui->stickyMenuBar->setVisible(true);
        } else {
            ui->stickyMenuBar->setVisible(false);
        }
    });


    // 스크롤 영역에서 마우스 제스처로 옆으로 움직일 수 있도록 설정
    QScroller::grabGesture(ui->scrollArea_option->viewport(), QScroller::LeftMouseButtonGesture);
    // 스크롤 비활성화
    ui->scrollArea_option->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_option->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}


MenuInfo* part8_menu_option_page::findMenuInTopData(const QString& targetName) {
    for (Category& cat : data.store_info) {
        for (StoreInfo& store : cat.restaurants) {
            for (MenuInfo& menu : store.menus) {
                if (menu.menu_name == targetName) {
                    return &menu;
                }
            }
        }
    }
}

void part8_menu_option_page::drawMenuOptions() {
    for (const MenuOption& opt : this->currentMenu->menu_options) {
        QString cat = opt.option_category;
        bool isRequired = opt.option_required == "Y";

        QFrame* frame = new QFrame;
        frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        QVBoxLayout* frameLayout = new QVBoxLayout(frame);
        frameLayout->setContentsMargins(10, 5, 10, 5);
        frameLayout->setSpacing(8);
        QLabel* title = new QLabel;
        QString titleText = QString("<b>%1</b>").arg(cat);
        if (isRequired) titleText += " <font color='red'>(필수 선택)</font>";
        title->setText(titleText);
        frameLayout->addWidget(title);

        QButtonGroup* group = isRequired ? new QButtonGroup(frame) : nullptr;
        if (group) group->setExclusive(true);

        for (const OptionInfo& option : opt.options) {
            QString label = option.option_name;
            if (option.option_price > "0")
                label += QString(" (+%1원)").arg(option.option_price);

            QAbstractButton* btn = isRequired
                                       ? static_cast<QAbstractButton*>(new QRadioButton(label))
                                       : static_cast<QAbstractButton*>(new QCheckBox(label));

            frameLayout->addWidget(btn);
            if (group) group->addButton(btn);
        }


        if (isRequired)
            requiredFrames[cat] = frame;
            requiredTitles[cat] = title;


        ui->layout_option->addWidget(frame);
    }
}

bool part8_menu_option_page::validateRequiredOptions() {
    for (auto it = requiredFrames.begin(); it != requiredFrames.end(); ++it) {
        QFrame* frame = it.value();
        bool isChecked = false;

        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(frame->layout());
        for (int i = 0; i < layout->count(); ++i) {
            QRadioButton* rb = qobject_cast<QRadioButton*>(layout->itemAt(i)->widget());
            if (rb && rb->isChecked()) {
                isChecked = true;
                break;
            }
        }

        if (!isChecked) {
            QLabel* title = requiredTitles[it.key()];
            if (title)
                title->setStyleSheet("background-color: yellow; padding: 3px; border-radius: 5px;");
            ui->scrollArea_option->ensureWidgetVisible(frame);
            return false;
        } else {
            QLabel* title = requiredTitles[it.key()];
            if (title)
                title->setStyleSheet("");
        }

    }
    return true;
}

void part8_menu_option_page::clearLayout(QLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* w = item->widget()) w->deleteLater();
        delete item;
    }
}


part8_menu_option_page::~part8_menu_option_page()
{
    delete ui;
}
