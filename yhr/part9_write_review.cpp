#include "part9_write_review.h"
#include "ui_part9_write_review.h"


part9_write_review::part9_write_review(TcpClient *client,QWidget *parent)
    : QWidget(parent), ui(new Ui::part9_write_review),client(client)
{


    ui->setupUi(this);
    ui->write_riw->setStyleSheet(
        "QPushButton {"
        "  border: 2px solid rgb(173, 216, 230);"
        "  color: rgb(17, 62, 245);"
        "  background-color: white;"
        "  border-radius: 6px;"     //  둥글게
        "  padding: 6px 12px;"      // 내부 여백
        "}"
        "QPushButton:hover {"
        "  background-color: rgb(230, 240, 255);"  // 호버 효과
        "}"
        "QPushButton:pressed {"
        "  background-color: rgb(200, 220, 255);"  // 클릭 효과
        "}"
    );

    ui->search_btn->setStyleSheet(
        "QPushButton {"
        "  border: 2px solid rgb(173, 216, 230);"
        "  color: rgb(17, 62, 245);"
        "  background-color: white;"
        "  border-radius: 6px;"     //  둥글게
        "  padding: 6px 12px;"      // 내부 여백
        "}"
        "QPushButton:hover {"
        "  background-color: rgb(230, 240, 255);"  // 호버 효과
        "}"
        "QPushButton:pressed {"
        "  background-color: rgb(200, 220, 255);"  // 클릭 효과
        "}"
        );
    ui->nopushbtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgb(245, 245, 245);"
        "  color: rgb(120, 120, 120);"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 6px 12px;"
        "}"
        );

    // 과거 주문 보기 - 준비중 선택
    ui->past_btn->setCheckable(true);
    ui->ready_btn->setCheckable(true);
    ui->past_btn->setAutoExclusive(true);
    ui->ready_btn->setAutoExclusive(true);

    connect(ui->BtnHome_2, &QPushButton::clicked, this, [=](){
        emit reqTurnToMain();
    });

    // 스크롤 영역에서 마우스 제스처로 옆으로 움직일 수 있도록 설정
    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
    // 스크롤 비활성화
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 스타일시트 지정
    QString btnStyle = R"(
    QPushButton {
        background-color: white;
        color: black;
        border: 1px solid #AAAAAA;
        padding: 6px 12px;
        border-radius: 6px;
    }
    QPushButton:checked {
        background-color: #5A8BFF;
        color: white;
    }
)";
    // 모든 버튼 checkable + autoExclusive 해제 (우리는 수동 제어할 거라 false)
    ui->past_btn->setCheckable(true);
    ui->ready_btn->setCheckable(true);
    ui->past_btn1->setCheckable(true);
    ui->ready_btn1->setCheckable(true);

    // 스타일시트 공통 적용
    QString btnStyle2 = R"(
    QPushButton {
        background-color: white;
        color: black;
        border: 1px solid #AAAAAA;
        padding: 6px 12px;
        border-radius: 6px;
    }
    QPushButton:checked {
        background-color: #5A8BFF;
        color: white;
    }
)";
    ui->past_btn->setStyleSheet(btnStyle2);
    ui->ready_btn->setStyleSheet(btnStyle2);
    ui->past_btn1->setStyleSheet(btnStyle2);
    ui->ready_btn1->setStyleSheet(btnStyle2);

    // 버튼 상태를 모두 갱신하는 헬퍼 함수
    auto updateButtons = [&](bool isPast) {
        ui->past_btn->setChecked(isPast);
        ui->past_btn1->setChecked(isPast);
        ui->ready_btn->setChecked(!isPast);
        ui->ready_btn1->setChecked(!isPast);
    };

    // 버튼 누르면 상태 업데이트 + 페이지 전환
    connect(ui->past_btn, &QPushButton::clicked, this, [=]() {
        updateButtons(true);
        ui->stackedWidget->setCurrentIndex(0);  // 과거 주문
    });
    connect(ui->ready_btn, &QPushButton::clicked, this, [=]() {
        updateButtons(false);
        ui->stackedWidget->setCurrentIndex(2);  // 준비 중
    });
    connect(ui->past_btn1, &QPushButton::clicked, this, [=]() {
        updateButtons(true);
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->ready_btn1, &QPushButton::clicked, this, [=]() {
        updateButtons(false);
        ui->stackedWidget->setCurrentIndex(2);
    });



    // 리뷰 작성하기/ 리뷰 확인하기 버튼
    updateButtons(true);  // 과거 주문 눌린 상태
    ui->stackedWidget->setCurrentIndex(0);

    //과거 주문내역 보기
    connect(ui->go_past_btn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    //리뷰 작성하기
    connect(ui->write_riw, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    // 1. 초기 상태: wri_btn 보이고 search_btn 숨기기
    ui->write_riw->show();
    ui->search_btn->hide();

    // 2. register_btn클릭하면 리뷰 등록
    connect(ui->register_btn, &QPushButton::clicked, this, [=]() {
        // 1번 페이지에서 0번 페이지로 이동
        ui->stackedWidget->setCurrentIndex(0);

        // 버튼 전환
        ui->write_riw->hide();
        ui->search_btn->show();

        QJsonObject req;
        req["signal"] = "make_review";
        req["role"] = "user";
        req["user_name"] = UserState::getInstance().userName;
        req["store_name"] = "교촌치킨";
        req["comment"] = ui->write_review->toPlainText();
        req["menu_name"] = "반반콤보";
        req["star_rank"] = "5";

        qDebug()<<UserState::getInstance().userName;
        client->sendJson(req);
        qDebug()<<req;


    });


    //리뷰작성하기에서 별점
    QVector<QPushButton*> foodStars = { ui->star_1, ui->star_2, ui->star_3, ui->star_4, ui->star_5 };
    QVector<QPushButton*> deliveryStars = { ui->star_6, ui->star_7, ui->star_8, ui->star_9, ui->star_10 };
    // 음식 별점
    for (int i = 0; i < foodStars.size(); ++i) {
        foodStars[i]->setStyleSheet("font-size: 32px; color: #DDDDDD; border: none; background: transparent;");
        connect(foodStars[i], &QPushButton::clicked, this, [=]() {
            for (int j = 0; j < foodStars.size(); ++j) {
                if (j <= i)
                    foodStars[j]->setStyleSheet("font-size: 32px; color: gold; border: none; background: transparent;");
                else
                    foodStars[j]->setStyleSheet("font-size: 32px; color: #DDDDDD; border: none; background: transparent;");
            }
        });
    }
    // 배달 별점
    for (int i = 0; i < deliveryStars.size(); ++i) {
        deliveryStars[i]->setStyleSheet("font-size: 32px; color: #DDDDDD; border: none; background: transparent;");
        connect(deliveryStars[i], &QPushButton::clicked, this, [=]() {
            for (int j = 0; j < deliveryStars.size(); ++j) {
                if (j <= i)
                    deliveryStars[j]->setStyleSheet("font-size: 32px; color: gold; border: none; background: transparent;");
                else
                    deliveryStars[j]->setStyleSheet("font-size: 32px; color: #DDDDDD; border: none; background: transparent;");
            }
        });
    }
    //리뷰 작성란
    ui->write_review->setPlaceholderText("주문하신 메뉴의 맛과 양에 대해 자세히 써주시면 더 유용한 리뷰가 돼요.");
    ui->write_review->setStyleSheet(R"(QTextEdit {padding: 4px;font-size: 10px;color: black;})");
    //혹시 입력한 리뷰를 사용해야한다면 QString text = ui->write_review->toPlainText();
    //글씨 회색으로 변경
    ui->gray1->setStyleSheet("color: #AAAAAA; font-size: 11px;");

    //x버튼
    ui->x_btn->setStyleSheet("border:none");
    connect(ui->x_btn, &QPushButton::clicked, this, [=](){
    ui->stackedWidget->setCurrentIndex(0);
    });

    // 좋아요 싫어요 버튼
    connect(ui->like_btn, &QPushButton::clicked, this, [=]() {
        ui->like_btn->setStyleSheet(
            "QPushButton {""  background-color: #00A9FF; border: none;border-radius: 10px;padding: 6px;""}" );
        ui->dislike_btn->setStyleSheet("");
    });

    connect(ui->dislike_btn, &QPushButton::clicked, this, [=]() {
        ui->dislike_btn->setStyleSheet(
            "QPushButton {""  background-color: #00A9FF; border: none;border-radius: 10px;padding: 6px;""}" );

        ui->like_btn->setStyleSheet("");
    });
    connect(ui->like_btn2, &QPushButton::clicked, this, [=]() {
        ui->like_btn2->setStyleSheet(
            "QPushButton {""  background-color: #00A9FF; border: none;border-radius: 10px;padding: 6px;""}" );
        ui->dislike_btn_2->setStyleSheet("");
    });

    connect(ui->dislike_btn_2, &QPushButton::clicked, this, [=]() {
        ui->dislike_btn_2->setStyleSheet(
            "QPushButton {""  background-color: #00A9FF; border: none;border-radius: 10px;padding: 6px;""}" );
        ui->like_btn2->setStyleSheet("");
    });

}

part9_write_review::~part9_write_review()
{
    delete ui;
}
