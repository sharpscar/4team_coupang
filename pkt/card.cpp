#include "card.h"
#include "ui_card.h"

Card::Card(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Card)
{
    ui->setupUi(this);
    this->setCursor(Qt::PointingHandCursor);
    this->setAttribute(Qt::WA_Hover);
    this->updateStyle(false);
    this->setFixedSize(400  , 200);

    doneButton = new QPushButton("조리 완료", this);
    doneButton->setObjectName("done_cooked");
    doneButton->setGeometry(205, 10 , 100,30);   // x, y w, h    

    doneButton->setStyleSheet(
        "QPushButton {"
        " background-color: #8C92AC;"
        " color: white;"
        " border: none;"
        " border-radius: 10px;"
        " padding: 8px;"
        " font-size: 12px;"
        " }"
        "QPushButton:hover {"
        " background-color: #A9A9A9;"
        " }"
        "QPushButton:pressed {"
        " background-color: #808080;"
        " }"
        );

    riderInfoButton = new QPushButton("라이더 정보", this);
    riderInfoButton->setObjectName("rider_info_button");
    riderInfoButton->setGeometry(205, 45, 100, 30);  // x, y, w, h
    riderInfoButton->hide();
    riderInfoButton->setStyleSheet(
        "QPushButton {"
        " background-color: #8C92AC;"
        " color: white;"
        " border: none;"
        " border-radius: 10px;"
        " padding: 8px;"
        " font-size: 12px;"
        " }"
        "QPushButton:hover {"
        " background-color: #A9A9A9;"
        " }"
        "QPushButton:pressed {"
        " background-color: #808080;"
        " }"
        );

    completeButton = new QPushButton("주문 처리 완료", this);
    completeButton->setObjectName("rider_info_button");
    completeButton->setGeometry(205, 80, 100, 30);  // x, y, w, h
    completeButton->hide();
    completeButton->setStyleSheet(
        "QPushButton {"
        " background-color: #8C92AC;"
        " color: white;"
        " border: none;"
        " border-radius: 10px;"
        " padding: 8px;"
        " font-size: 12px;"
        " }"
        "QPushButton:hover {"
        " background-color: #A9A9A9;"
        " }"
        "QPushButton:pressed {"
        " background-color: #808080;"
        " }"
        );

    connect(doneButton, &QPushButton::clicked, this, [=](){
        emit doneButtonClicked();
    });

}

void Card::mousePressEvent(QMouseEvent* e){
    if (e->button() == Qt::LeftButton) {
        emit clicked();

    }
    QWidget::mousePressEvent(e);
}

void Card::enterEvent(QEnterEvent* e){
    this->updateStyle(true);
    QWidget::enterEvent(e);
}

void Card::leaveEvent(QEvent* e) {
    this->updateStyle(false);
    QWidget::leaveEvent(e);
}


void Card::updateStyle(bool h){
    if (h) {
        this->setStyleSheet("background-color: #e0f0ff; border: 1px solid #3399ff; color: black;");
    } else {
        this->setStyleSheet("background-color: white; border: 1px solid lightgray; color: black;");
    }
}
// 매개변수에 서버로부터 받은 구조체를 이용해 각각의 ui-> 라벨에 setText()한다.
void Card::setText(const QString& text) {
    // qDebug()<< "setText 함수가 호출되었다." <<text;
    ui->card_label->setText(text);



    ui->card_label->setEnabled(false);
    ui->card_label->setAlignment(Qt::AlignLeft| Qt::AlignTop);
}

/*
 * 라이더 정보를 설정한다.
 */
void Card::set_rider_Text(const QString& text) {

    this->riderInfo_.text = text;
    riderInfoButton->show();

    connect(riderInfoButton, &QPushButton::clicked, this, [=](){
        QMessageBox::information(this, "라이더정보", this->riderInfo_.text);
    });
}


// 주문완료를 버튼으로 표시한다.
void Card::set_complete_Text(const QString& text) {
    completeButton->show();
    qDebug()<<text<<":  완료완료!!!";
}

Card::~Card()
{
    delete ui;
}
