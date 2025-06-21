#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QThread>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new TcpClient(this);
    client->connectToServer();



    loginPage = new login(client,this);
    signupPage = new sign_up(client, this);
    dashboardPage = new Dashboard(client, this);
    menuManagePage = new MenuManage(client, this);
    orderCheckPage = new Ordercheck(client, this);
    timeManagePage = new TimeManage(client, this);


    // 초기화면을 설정
    initializeStackedWidget();

    // 서버에서 받는 내용에 따라 정보를 구조체에 담고 담은 내용을 각 페이지 객체에 담아 생성한다.
    connect(client, &TcpClient::jsonReceived, this, [this](QJsonObject json){

        QString signal = json["signal"].toString();

        // 로그인 처리
        if(signal=="response_store_login"){ //check_duplicate_id_result
            QString login_result = json["store_login_result"].toString();

            if(login_result =="fail" ){
                QMessageBox::warning(this, "알림", " 로그인에 실패하였습니다. ");
            }
            else if(login_result=="success"){
                qDebug()<<"로그인 응답 받았습니다. \n";
                this->get_menu_infomation(json);
                //로그인이 되었을 경우에만  가게정보를 파싱한다.
                menuManagePage->set_store_info(this->store);
                saveJsonToFile(json);
                QMessageBox::information(this, "알림", "로그인이 되었습니다.");
                ui->main_widget->addWidget(dashboardPage);
                ui->main_widget->setCurrentWidget(dashboardPage);                
                ui->main_widget->update();
            }
        }else if(signal=="order_accept") {

            //오더 정보가 오면 하나의 오더를 파싱한다.
            get_order_infomation(json);

            //테스트삼아 json 저장
            saveJsonToFile(json);

            // 전체 오더에 PREPEND 한다.
            orderCheckPage->add_order_info(orders_);
            OrderResDialog *dlg =  new OrderResDialog(orders_, this);
            ui->main_widget->setCurrentWidget(orderCheckPage);
            if(dlg->exec() ==QDialog::Accepted){
                int selectedTime = dlg->getResponseTime();
                QJsonObject req;
                if(selectedTime != -1){
                    qDebug()<<"선택된 응답시간:" <<selectedTime;
                    req["signal"]= "res_order_accept";
                    req["role"] = "store";
                    req["store_name"] = "KYOCHON";
                    req["accept"]="yes";
                    req["order_serialNumber"] = orders_.order_serialNumber;
                    req["time_take"]= QString::number(selectedTime);

                }else if(selectedTime ==-1){
                    req["signal"]= "res_order_accept";
                    req["role"] = "store";
                    req["accept"]="no";

                }
                // 서버에  오더 수락/ 거절 정보 전송
                client->sendJson(req);
                // 오더체크로 이동

                // 카드정보를 설정함
                orderCheckPage->setCards();

                ui->main_widget->setCurrentWidget(orderCheckPage);
            }
        /// 수락 거절 신호끝
        }

        else if(signal=="res_rider_info"){

            //라이더 정보를 받으면 뭘해야할까?
            QString p_num = json["p_num"].toString();
            QString name = json["name"].toString();
            QString order_serialNumber = json["order_num"].toString();
            // QMessageBox::information(this, "라이더가 배정되었습니다.. ",order_serialNumber );

            QString rider_info ="이름 : "+name + " 전화번호 : "+p_num;
            emit riderInfoReceived(order_serialNumber, rider_info);

            //로그인 화면으로 이동하는것 막기
            ui->main_widget->setCurrentWidget(orderCheckPage);

            }else if(signal=="delivery_end"){

                QString order_num  = json["order_num"].toString();
                // QMessageBox::information(this, "음식이 달되었습니다. ",order_num );
                QString com_text =  order_num + ":  완료 ";
                // card->set_complete_Text(order_num);
                Card* targetCard = m_cards.value(order_num);

                if (targetCard){

                    targetCard->set_complete_Text(com_text);
                }
                emit completeInfoReceived(order_num);
                //로그인 화면으로 이동하는것 막기
                ui->main_widget->setCurrentWidget(orderCheckPage);
            }

    });



    // 아까 생성한 오더체크 페이지를 참조
    connect(this, &MainWindow::riderInfoReceived, orderCheckPage, &Ordercheck::onRiderInfoReceived);

    connect(this, &MainWindow::completeInfoReceived, orderCheckPage, &Ordercheck::onCompleteInfoReceived);

    // 회원가입 시그널을 듣고 페이지 이동
    connect(signupPage, &sign_up::req_turn_to_login, this, [=](){
        ui->main_widget->setCurrentWidget(loginPage);
    });

    //로그인후 메인 페이지로 이동
    connect(loginPage, &login::req_turn_to_dash, this, [=](){
        ui->main_widget->setCurrentWidget(dashboardPage);
    });

    // 메뉴관리에서 대시보드로 이동
    connect(menuManagePage, &MenuManage::req_turn_to_dashboard, this, [=](){
        ui->main_widget->setCurrentWidget(dashboardPage);
    });

    // 메뉴관리에서 오더 체크로 이동
    connect(menuManagePage, &MenuManage::req_turn_to_ordercheck, this, [=](){
        ui->main_widget->setCurrentWidget(orderCheckPage);
    });

    // 메뉴관리에서 영업시간 관리로 이동
    connect(menuManagePage, &MenuManage::req_turn_to_timeManage, this, [=](){
        ui->main_widget->setCurrentWidget(timeManagePage);
    });

    // 영업시간 관리에서 대시보드로 이동
    connect(timeManagePage, &TimeManage::req_turn_to_dashboard, this, [=](){
        ui->main_widget->setCurrentWidget(dashboardPage);
    });


    // 영업시간 관리에서 메뉴 관리로 이동
    connect(timeManagePage, &TimeManage::req_turn_to_menuManage, this, [=](){
        ui->main_widget->setCurrentWidget(menuManagePage);
    });

    // 영업시간 관리에서 메뉴 관리로 이동
    connect(timeManagePage, &TimeManage::req_turn_to_ordercheck, this, [=](){
        ui->main_widget->setCurrentWidget(orderCheckPage);
    });

    // 오더 체크에서 대시보드로 이동
    connect(orderCheckPage, &Ordercheck::req_turn_to_dashboard, this, [=](){
        ui->main_widget->setCurrentWidget(dashboardPage);
    });


    // 오더체크에서 영업시간 관리로 이동
    connect(orderCheckPage, &Ordercheck::req_turn_to_timeManage, this, [=](){
        ui->main_widget->setCurrentWidget(timeManagePage);
    });

    // 오더체크에서 메뉴 관리로 이동
    connect(orderCheckPage, &Ordercheck::req_turn_to_menuManage, this, [=](){
        ui->main_widget->setCurrentWidget(menuManagePage);
    });




    // 회원가입으로 이동!
    connect(loginPage, &login::req_turn_to_signup, this, [=](){
        ui->main_widget->setCurrentWidget(signupPage);
    });

    // 대시보드에서 -> 메뉴관리로 이동!
    connect(dashboardPage, &Dashboard::req_turn_to_menu, this, [=](){
        ui->main_widget->setCurrentWidget(menuManagePage);
    });

    //대시보드에서 영업시간관리로 이동
    connect(dashboardPage, &Dashboard::req_turn_to_timeManage, this, [=](){
        ui->main_widget->setCurrentWidget(timeManagePage);
    });



    connect(dashboardPage, &Dashboard::req_turn_to_login, this, [=](){
        ui->main_widget->setCurrentWidget(loginPage);
    });

    //오더 체크 페이지로 이동
    connect(dashboardPage, &Dashboard::req_turn_to_order, this, [=](){
        ui->main_widget->setCurrentWidget(orderCheckPage);
    });



}

//모든 주문정보를 받는 부분 (아래 함수 반복)   -> 전체 테스트시 get_order_infomation 이 함수 호출
void MainWindow::get_all_order_infomation(const QJsonObject& json){

    QJsonArray jsonArray = json["order_info"].toArray();
    if (jsonArray.size()>0)
    {
        for(const QJsonValue& val : jsonArray){

            QJsonObject obj = val.toObject();
            Orders o = parseOrder(obj);
            allOrders.append(o);
        }
    }
}


// 하나의 주문을 받는다!
Orders MainWindow::parseOrder(const QJsonObject& json){

    orders_.order_serialNumber =  json["order_serialNumber"].toString();
    QJsonArray order_itmes_array = json["order_items"].toArray();
    orders_.store_name = json["store_name"].toString();
    orders_.order_status = json["order_status"].toString();
    orders_.order_time = json["order_time"].toString();
    orders_.to_owner = json["to_owner"].toString();
    orders_.to_rider =  json["to_rider"].toString();
    orders_.user_addr = json["user_addr"].toString();
    orders_.user_pnum = json["user_pnum"].toString();
    orders_.total_price = json["total_price"].toString();
    orders_.eat_type = json["eat_type"].toString();
    orders_.once_plastic = json["once_plastic"].toString();

    int cnt =0;
    // 여러개의 메뉴명을 하나의 변수에 담기위한 변수
    QString temp;
    for (const QJsonValue& order_item : order_itmes_array) {
        QJsonObject order_Obj = order_item.toObject();

        oi_.menu_name = order_Obj["menu_name"].toString();
        oi_.option = order_Obj["option"].toString();
        oi_.price = order_Obj["price"].toString();
        oi_.quantity = order_Obj["quantity"].toString();
        orders_.order_Items.append(oi_);

        if(!temp.isEmpty()){
            temp += ", ";
        }
        temp += oi_.menu_name;
        cnt++;
    }

    orders_.menu_quentity = QString::number(cnt);
    orders_.order_items_menu_names = temp;
    // qDebug()<<"get_order_infomation 함수끝부분  "<<orders_.order_items_menu_names;

    return orders_;

}


//하나의 주문정보가 왔을때 이 함수를 호출한다.
void MainWindow::get_order_infomation(const QJsonObject& json){


    QJsonObject orderObject;
    QJsonArray order_itmes_array;


    orderObject = json["orders"].toObject();

    order_itmes_array = orderObject["order_items"].toArray();
    order_itmes_array = orderObject["order_items"].toArray();

    if(!orderObject.isEmpty()){
        orders_.order_serialNumber =  orderObject["order_serialNumber"].toString();
        orders_.store_name = orderObject["store_name"].toString();
        orders_.order_status = orderObject["order_status"].toString();
        orders_.order_time = orderObject["order_time"].toString();
        orders_.to_owner = orderObject["to_owner"].toString();
        orders_.to_rider =  orderObject["to_rider"].toString();
        orders_.user_addr = orderObject["user_addr"].toString();
        orders_.user_pnum = orderObject["user_pnum"].toString();
        orders_.total_price = orderObject["total_price"].toString();
        orders_.eat_type = orderObject["eat_type"].toString();
        orders_.once_plastic = orderObject["once_plastic"].toString();

        int cnt =0;
        QString temp;
        for (const QJsonValue& order_item : order_itmes_array) {
            QJsonObject order_Obj = order_item.toObject();

            oi_.menu_name = order_Obj["menu_name"].toString();
            oi_.option = order_Obj["option"].toString();
            oi_.price = order_Obj["price"].toString();
            oi_.quantity = order_Obj["quantity"].toString();
            orders_.order_Items.append(oi_);

            if(!temp.isEmpty()){
                temp += ", ";
            }
            temp += oi_.menu_name;
            cnt++;
        }

        orders_.menu_quentity = QString::number(cnt);
        orders_.order_items_menu_names = temp;
        // qDebug()<<"get_order_infomation 함수끝부분  "<<orders_.order_items_menu_names;

    }



}

void MainWindow::get_menu_infomation(const QJsonObject& json){


    //OptionInfo(옵션정보), MenuOption(옵션 카테고리 정보), MenuInfo(메뉴정보) ,StoreInfo(식당정보)
    QJsonArray storeArray = json["store_info"].toArray(); // 스토어 정보를 받는다

    QJsonObject storeObject = storeArray[0].toObject();

    if(!storeObject.isEmpty())
    {
        // 25.06.16 store 만 변수화 함
        // store;

        store.store_name = storeObject["store_name"].toString();
        store.category_num = storeObject["category_num"].toInt();
        store.store_addr = storeObject["store_addr"].toString();
        store.phone_num = storeObject["phone_num"].toString();
        store.owner_name = storeObject["owner_name"].toString();
        store.opening_time = storeObject["opening_time"].toString();
        store.closing_time = storeObject["closing_time"].toString();
        store.delivery_fee = storeObject["delivery_fee"].toString();
        store.minimum_order = storeObject["minimum_order"].toString();
        store.store_desc = storeObject["store_desc"].toString();
        store.origin_info = storeObject["origin_info"].toString();
        store.nutrition_fact = storeObject["nutrition_fact"].toString();
        store.allergen_info = storeObject["allergen_info"].toString();
        //메뉴 정보
        QJsonObject menu_info_obj = storeObject["menu_info"].toObject();

        QJsonArray menusArr = menu_info_obj["menus"].toArray();        

        for (const QJsonValue& menuVal : menusArr) {
            QJsonObject menuObj = menuVal.toObject();
            MenuInfo menu;
            menu.menu_category = menuObj["menu_category"].toString();
            menu.menu_name = menuObj["menu_name"].toString();
            menu.menu_info = menuObj["menu_info"].toString();
            menu.menu_img_path = menuObj["menu_img_path"].toString();
            menu.menu_status = menuObj["menu_status"].toString();
            menu.menu_price= menuObj["menu_price"].toString();

            QJsonArray menuOptionsArr = menuObj["menu_options"].toArray();
            for (const QJsonValue& optCatVal : menuOptionsArr) {
                QJsonObject optCatObj = optCatVal.toObject();
                MenuOption menuOption;
                menuOption.option_category = optCatObj["option_category"].toString();
                menuOption.option_required = optCatObj["option_required"].toString();

                // qDebug()<< "menuOption.option_category "  <<menuOption.option_category << "\n";

                // 실제 옵션 배열
                QJsonArray optionsArr = optCatObj["options"].toArray();
                for (const QJsonValue& optVal : optionsArr) {
                    QJsonObject optObj = optVal.toObject();
                    OptionInfo option;
                    option.option_name = optObj["option_name"].toString();
                    option.option_price = optObj["option_price"].toInt();
                    option.option_status = optObj["option_status"].toString();

                    menuOption.options.append(option);
                }
                menu.menu_options.append(menuOption);
            }
            store.menus.append(menu);
        }
    }

}

void MainWindow::saveJsonToFile(const QJsonObject& json) {
    // 파일 이름에 타임스탬프를 추가해서 덮어쓰지 않게
    QString fileName = QString("server_response_%1.json")
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("파일 열기 실패: %s", qPrintable(file.errorString()));
        return;
    }

    QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));  // 보기 좋게 들여쓰기해서 저장
    file.close();

    qDebug() << "JSON 데이터 저장 완료:" << fileName;
}



void MainWindow::initializeStackedWidget(){
    ui->main_widget->addWidget(dashboardPage);
    ui->main_widget->addWidget(loginPage);
    ui->main_widget->addWidget(signupPage);
    ui->main_widget->addWidget(menuManagePage);
    ui->main_widget->addWidget(orderCheckPage);
    ui->main_widget->addWidget(timeManagePage);
    ui->main_widget->setCurrentWidget(loginPage);

}

MainWindow::~MainWindow()
{
    delete ui;
}
