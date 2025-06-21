#include "part6_order_complete.h"
#include "ui_part6_order_complete.h"


using namespace std;

part6_order_complete::part6_order_complete(TcpClient* client, QWidget *parent)
    : QWidget(parent), ui(new Ui::part6_order_complete), client(client)
{
    ui->setupUi(this);
    //시그널 받아서 페이지 넘겨주기
    connect(client, &TcpClient::jsonReceived, this, [=](QJsonObject json){
        qDebug() << "확인: " << json;
        QString signal = json["signal"].toString();
        if (signal == "order_pass") {
            QString message = json["result"].toString();
            {
                if(message == "yes")
                {
                    // order_num
                    // cooking_time
                    this->order_num = json["order_num"].toString();
                    this->cooking_time = json["cooking_time"].toString();
                    qDebug() << "주문번호: " << order_num;
                    qDebug() << "주문시간: " << cooking_time;

                    ui->label_order_num->setText(this->order_num);
                    ui->order_num->setText(this->order_num);
                    ui->order_num2->setText(this->order_num);
                    ui->order_num3->setText(this->order_num);

                    ui->stackedWidget->setCurrentWidget(ui->page_cooking);
                }
            }
        }
        else if (signal == "pick_up_start") {

            qDebug()<<"픽업 완료";
            ui->stackedWidget->setCurrentWidget(ui->page_delivery);
        }
        else if (signal == "delivery_end") {
            qDebug()<<"배달 완료";
            ui->stackedWidget->setCurrentWidget(ui->done_delivery);
        }
    });


    ui->for_skyblue->setStyleSheet("color: skyblue;");

    connect(ui->bh_btn, &QPushButton::clicked, this, &part6_order_complete::on_myButton_clicked);
    //수정 버튼 누르면 해당 버튼 ui클리어
    connect(ui->road_modify, &QPushButton::clicked, this, &part6_order_complete::modify_clear);

    //길안내 동적생성해야함
    ui->road_guide->setPlaceholderText("경비실 지나서 쭉 들어와주세요 ");

    //x버튼 테두리 없애기
    ui->x_btn->setStyleSheet("QPushButton {border:none;};");
    ui->x_btn2->setStyleSheet("QPushButton {border:none;};");
    ui->x_btn3->setStyleSheet("QPushButton {border:none;};");
    //도움말 버튼 스타일링
    ui->bh_btn->setStyleSheet("QPushButton {background-color: #D3D3D3;border: none;color: black; border-radius: 30px;}");
    ui->bh_btn_2->setStyleSheet("QPushButton {background-color: #D3D3D3;border: none;color: black;border-radius: 30px;}");
    ui->bh_btn_3->setStyleSheet("QPushButton {background-color: #D3D3D3;border: none;color: black;border-radius: 30px;}");



    // 메인화면으로 돌아가기
    connect(ui->x_btn, &QPushButton::clicked, this, [=](){
        emit reqToMainPage();
    });
    connect(ui->x_btn2, &QPushButton::clicked, this, [=](){
        emit reqToMainPage();
    });
    connect(ui->x_btn3, &QPushButton::clicked, this, [=](){
        emit reqToMainPage();
    });



}



//병희님 연락처로 연결
void part6_order_complete::on_myButton_clicked() {
    QMessageBox::information(this, "server_num", "010-0000-0000!");
}
//수정 버튼 누르면 해당 버튼 ui클리어
void part6_order_complete::modify_clear()
{
    ui->road_guide->clear();
}


part6_order_complete::~part6_order_complete()
{
    delete ui;
}
