#include "w_v_addr_card.h"
#include "ui_w_v_addr_card.h"

w_v_addr_card::w_v_addr_card(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::w_v_addr_card)
{
    ui->setupUi(this);

    ui->frame_select_addr->setCursor(Qt::PointingHandCursor);
    ui->frame_select_addr->setAttribute(Qt::WA_Hover);
    ui->frame_select_addr->installEventFilter(this);
}

bool w_v_addr_card::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->frame_select_addr) {
        if (event->type() == QEvent::MouseButtonPress) {
            qDebug() << "frame_select_addr 클릭됨!";
            // 원하는 동작 추가
            emit frameClicked();
            return true;
        }
        else if (event->type() == QEvent::Enter) {
            updateFrameStyle(true);
            return true;
        }
        else if (event->type() == QEvent::Leave) {
            updateFrameStyle(false);
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void w_v_addr_card::updateFrameStyle(bool hover) {
    if (hover) {
        ui->frame_select_addr->setStyleSheet(
            "QFrame {"
            "background-color: #f0faff;"
            "border: 1px solid #66aaff;"
            "border-radius: 6px;"
            "}"
            );
    } else {
        ui->frame_select_addr->setStyleSheet(
            "QFrame {"
            "background-color: white;"
            "border: 1px solid lightgray;"
            "border-radius: 6px;"
            "}"
            );
    }
}

w_v_addr_card::~w_v_addr_card()
{
    delete ui;
}



