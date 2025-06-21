#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow( QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    client = new TcpClient(this);  // MainWindow 소멸 시 같이 삭제됨
    client->connectToServer();     // 연결은 딱 한 번만 하면 됨

    ui->store_name_show->setWordWrap(true);
    ui->store_name->setWordWrap(true);
    ui->store_addr->setWordWrap(true);
    ui->request->setWordWrap(true);
    ui->coust_addr->setWordWrap(true);
    ui->request_2->setWordWrap(true);


    //시그널 받아서 페이지 넘겨주기
    connect(client, &TcpClient::jsonReceived, this, [=](QJsonObject json){
        qDebug()<<json;
        QString signal = json["signal"].toString();
        if (signal == "yoo_rider") {
            ui->waiting->hide();
            ui->store_name_show->show();
            ui->order_num_show->show();
            ui->no_btn->show();
            ui->yes_btn->show();
            this-> message = json["result"].toString();
            this-> store_name = json["store_name"].toString();
            this-> order_num = json["delivery_num"].toString();
            {
                if(message == "yes_or_no")
                {
                    ui->stackedWidget->setCurrentIndex(2);
                    ui->store_name_show->setText(store_name);
                    ui->order_num_show->setText(order_num);
                }
            }
        }
        else if (signal == "ok_and_info")  //수락 누르면
        {
            //구조체에 저장한다.
            this-> message = json["result"].toString();
            QJsonObject rider_info = json["result"].toObject();
            currentrider.store_name = rider_info["store_name"].toString();
            currentrider.store_addr = rider_info["store_addr"].toString();
            currentrider.order_num = rider_info["order_num"].toString();
            currentrider.to_rider = rider_info["to_rider"].toString();
            currentrider.total_price = rider_info["total_price"].toString();
            currentrider.user_addr = rider_info["user_addr"].toString();
            currentrider.user_pnum = rider_info["user_pnum"].toString();

            ui->stackedWidget->setCurrentIndex(3);

            ui->store_name->setText(currentrider.store_name);
            qDebug()<<"가게 이름 : "<<currentrider.store_name;
            ui->store_addr->setText(currentrider.store_addr);
            qDebug()<< "가게 주소 : "<<currentrider.store_addr;
            ui->order_num->setText(currentrider.order_num);
            ui->request->setText(currentrider.to_rider);
        }
        else if (signal == "login_result")
        {
            QString message = json["result"].toString();
            if(message == "success")
            {    QMessageBox::information(this, "로그인 완료", "배달 대기화면으로\n 전환됩니다!");
                ui->stackedWidget->setCurrentIndex(2);
            }
        }
    });
    //로그인 UI=================
    ui->makeid_btn->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px;");
    ui->login_btn->setStyleSheet("QPushButton {background-color: #FFFFFF;color: #2E8B57;border: 2px solid #2E8B57;border-radius: 8px;padding: 6px 12px;}");
    ui->go_login->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px;");
    ui->insert_pnum->setPlaceholderText("휴대폰 번호를 입력해주세요");
    ui->insert_pw->setPlaceholderText("비밀번호를 입력해주세요");
    ui->insert_pw->setEchoMode(QLineEdit::Password);
    ui->login_text->setStyleSheet("QPushButton {background-color: #FFFFFF;color: #2E8B57;border: 2px solid #2E8B57;border-radius: 8px;padding: 6px 12px;}");
    //=====================
    //====나머지 버튼 UI
    ui->yes_btn->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px;");
    ui->finish_pickup->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px;");
    ui->finish_dil->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px;");

    //======여기서 부터 배달 =====
    ui->store_name_show->hide();
    ui->order_num_show->hide();
    ui->no_btn->hide();
    ui->yes_btn->hide();
    ui->waiting->show();


    //배달 수락 버튼 눌렀을 때
    connect(ui->yes_btn, &QPushButton::clicked, this, [=](){
        //라이더 수락  테스트
        QJsonObject req;
        req["signal"] = "yoo_rider_yes";
        req["rider_pnum"] = currentrider.user_pnum;
        req["rider_name"] = "문재윤";
        req["order_num"] = this->order_num;
        // 전송
        client->sendJson(req);

        ui->stackedWidget->setCurrentIndex(3);
       });

    //픽업 버튼 눌렀을 때
    connect(ui->finish_pickup, &QPushButton::clicked, this, [=](){
        QJsonObject req;
        req["signal"] = "pick_up";
        req["order_num"] = this->order_num;
        client->sendJson(req);

        //pickup버튼 누른 뒤 UI변경
        ui->stackedWidget->setCurrentIndex(4);
        ui->coust_addr->setText(currentrider.user_addr);
        ui->order_num_2->setText(currentrider.order_num);
        ui->request_2->setText(currentrider.to_rider);
    });
    // //배달 완료 버튼 눌렀을 떄
    connect(ui->finish_dil, &QPushButton::clicked, this, [=](){

        QMessageBox::information(this, "배달 완료", "배달이 완료되었습니다!");
        QJsonObject req;
        req["signal"] = "delivery_end";
        req["order_num"] = this->order_num;
        req["pnum"] = currentrider.user_pnum;
        client->sendJson(req);

        ui->stackedWidget->setCurrentIndex(2);

    });
    //====로그인 및 화면 연결===
    //로그인 버튼 눌렀을 때
    connect(ui->go_login, &QPushButton::clicked, this, [=](){
        QJsonObject req;
        req["signal"] = "login";
        req["role"] = "rider";
        req["pnum"] = ui->insert_pnum->text();
        req["pw"] = ui->insert_pw->text();
        client->sendJson(req);
    });
    //로그인 화면에서 뒤로가기
    connect(ui->return_btn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    //로그인 화면으로 가기
    connect(ui->login_btn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
    });



}




MainWindow::~MainWindow()
{
    delete ui;
}
