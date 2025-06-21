#include "my_page.h"
#include "ui_my_page.h"

my_page::my_page(TcpClient *client, QWidget *parent)
    : QWidget(parent), ui(new Ui::my_page), client(client)

{
    ui->setupUi(this);

    connect(ui->notmean2, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(2);});
    connect(ui->for_payment, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(1);});
    connect(ui->nomean3, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(3);});
    connect(ui->notmean7, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(7);});
    connect(ui->nomean8, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(6);});
    connect(ui->notmean8, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(8);});
    connect(ui->nomean6, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(5);});
    connect(ui->setting, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(4);});


    connect(ui->logout_btn, &QPushButton::clicked, this, [=](){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "",
                                      "정말 로그아웃 하시겠습니까?",
                                      QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::No) {this->close();}
        else {UserState::getInstance().logout();}
    });

    //내정보보기 메인으로 이동
    connect(ui->go_info_main, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(0);});
    connect(ui->go_info_main_2, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(0);});
    connect(ui->go_info_main_3, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(0);});
    connect(ui->go_info_main_4, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(0);});
    connect(ui->go_info_main_5, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(0);});
    connect(ui->go_info_main_6, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(0);});
    connect(ui->go_info_main_7, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(0);});
    connect(ui->go_info_main_8, &QPushButton::clicked, this, [=]() {ui->info_menu->setCurrentIndex(0);});

    ui->label_name->setText(UserState::getInstance().userName);

    //버튼
    ui->switch_2->setStyleSheet(R"(
    QCheckBox::indicator {
        width: 50px;
        height: 28px;
        border-radius: 14px;
        background-color: lightgray;
    }
    QCheckBox::indicator:checked {
        background-color: #00BFFF; /* 하늘색 */
        border: 1px solid #009ACD;
    }
)");

    ui->switch_3->setStyleSheet(R"(
    QCheckBox::indicator {
        width: 50px;
        height: 28px;
        border-radius: 14px;
        background-color: lightgray;
    }
    QCheckBox::indicator:checked {
        background-color: #00BFFF; /* 하늘색 */
        border: 1px solid #009ACD;
    }
)");
    ui->switch_4->setStyleSheet(R"(
    QCheckBox::indicator {
        width: 50px;
        height: 28px;
        border-radius: 14px;
        background-color: lightgray;
    }
    QCheckBox::indicator:checked {
        background-color: #00BFFF; /* 하늘색 */
        border: 1px solid #009ACD;
    }
)");


    ui->qbtn->setStyleSheet("color: skyblue;");
    ui->qbtn1->setStyleSheet("color: skyblue;");
    ui->qbtn2->setStyleSheet("color: skyblue;");
    ui->qbtn_3->setStyleSheet("color: skyblue;");
    ui->qbtn_4->setStyleSheet("color: skyblue;");
    ui->qbtn_5->setStyleSheet("color: skyblue;");






}


my_page::~my_page()
{
    delete ui;
}
