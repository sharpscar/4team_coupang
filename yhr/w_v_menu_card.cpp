#include "w_v_menu_card.h"
#include "ui_w_v_menu_card.h"

w_v_menu_card::w_v_menu_card(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::w_v_menu_card)
{
    ui->setupUi(this);

    ui->m_photo->setAlignment(Qt::AlignCenter);
    ui->m_photo->setStyleSheet("border: 1px solid #ddd; margin: 5px;");
    ui->horizontalLayout->setContentsMargins(5, 5, 5, 5);
    ui->horizontalLayout->setSpacing(8);

    ui->m_explain->setWordWrap(true);

    QTimer::singleShot(0, this, [=]() {
        QLabel* cartIcon = new QLabel(ui->m_photo);  // 부모를 m_photo로

        cartIcon->setText("+");
        cartIcon->setAlignment(Qt::AlignCenter);
        cartIcon->setFixedSize(30, 30);
        cartIcon->setStyleSheet(
            "background-color: #2196F3;"  // 파란색 배경
            "color: white;"               // 흰색 글자
            "border-radius: 10px;"
            "font-weight: bold;"
            "font-size: 14px;"
            "border: none;"
            );

        // 우하단에 배치
        cartIcon->move(ui->m_photo->width() - cartIcon->width() - 4,
                       ui->m_photo->height() - cartIcon->height() - 4);
        cartIcon->raise();
        cartIcon->show();
    });



}

void w_v_menu_card::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        emit clicked();
    }
    QWidget::mousePressEvent(e);
}

void w_v_menu_card::enterEvent(QEnterEvent* e) {
    this->updateStyle(true);
    QWidget::enterEvent(e);
}

void w_v_menu_card::leaveEvent(QEvent* e) {
    this->updateStyle(false);
    QWidget::leaveEvent(e);
}

void w_v_menu_card::updateStyle(bool h) {
    if (h) {
        this->setStyleSheet("background-color: #e0f0ff; border: 1px solid #3399ff;");
    } else {
        this->setStyleSheet("background-color: white; border: 1px solid lightgray;");
    }
}


w_v_menu_card::~w_v_menu_card()
{
    delete ui;
}
