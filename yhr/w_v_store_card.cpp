#include "w_v_store_card.h"
#include "ui_w_v_store_card.h"

w_v_store_card::w_v_store_card(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::w_v_store_card)
{
    ui->setupUi(this);

    this->setCursor(Qt::PointingHandCursor);
    this->setAttribute(Qt::WA_Hover);

    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->ad_label->setText("wow 매주문 무료배달 적용매장");
    ui->ad_label->setAlignment(Qt::AlignCenter);
    ui->ad_label->setStyleSheet(
        "QLabel {"
        " color: white;"              // 글자 색
        " background-color: #003366;" // 남색 배경
        " border-radius: 8px;"        // 둥근 모서리R
        " padding: 6px 12px;"         // 내부 여백
        " font-size: 12px;"           //글자크기 조절
        " font-weight: bold;"         // 글자 강조
        "}"
        );
}

void w_v_store_card::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        emit clicked();
    }
    QWidget::mousePressEvent(e);
}

void w_v_store_card::enterEvent(QEnterEvent* e) {
    this->updateStyle(true);
    QWidget::enterEvent(e);
}

void w_v_store_card::leaveEvent(QEvent* e) {
    this->updateStyle(false);
    QWidget::leaveEvent(e);
}

void w_v_store_card::updateStyle(bool h) {
    if (h) {
        this->setStyleSheet("background-color: #e0f0ff; border: 1px solid #3399ff; border-radius: 10px;");
    } else {
        this->setStyleSheet("background-color: white; border: 1px solid lightgray; border-radius: 10px;");
    }
}


w_v_store_card::~w_v_store_card()
{
    delete ui;
}


