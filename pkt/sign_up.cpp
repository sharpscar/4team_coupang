#include "sign_up.h"
#include "ui_sign_up.h"

sign_up::sign_up(TcpClient * client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sign_up), client(client)
{
    ui->setupUi(this);
    QMap<QString, QStringList> regionMap;
    setRegion(regionMap);

    // 시/도 콤보박스 초기화
    ui->CityCombo->addItems(regionMap.keys());
    // 시/도 선택 시 구/군 콤보박스 변경
    connect(ui->CityCombo, &QComboBox::currentTextChanged, this, [=](const QString &sido){
        ui->GuGunCombo->clear();
        ui->GuGunCombo->addItems(regionMap[sido]);
    });
    //구 군 처리
    connect(ui->GuGunCombo, &QComboBox::currentTextChanged, this, [=](const QString &gugun){
        QString currentText = ui->deliver_area_le->text();
        //중복 추가 ㄴㄴ
        if(!currentText.contains(gugun)){
            if(!currentText.isEmpty()){
                currentText+=", ";
            }
            currentText += gugun;
            ui->deliver_area_le->setText(currentText);
        }
        // ui->deliver_area_le->setText(gugun);
    });
    // 최초 초기화
    ui->GuGunCombo->addItems(regionMap[ui->CityCombo->currentText()]);


    //회원 가입 버튼을 누르면 각 요소의 값을 가져와서 json 생성 함수 호출 (커넥트 함수 연결)
    connect(ui->signup_btn,  &QPushButton::clicked, this, &sign_up::do_store_signup);

    //중복검사 버튼을 누르면 서버로 아이디를 체크한다.
    connect(ui->btn_check_duplicate, &QPushButton::clicked, this, &sign_up::do_check_duplicate);

    // 가입버튼 누르고 나서 서버로부터 받은 json 응답을 처리
    connect(client, &TcpClient::jsonReceived, this, [=](QJsonObject json){
        QString signal = json["signal"].toString();

        if(signal=="check_id_result"){ //check_duplicate_id_result

            qDebug()<<"응답 받았습니다. \n";
            QString message = json["result"].toString();
            if(message=="success"){
                ui->btn_check_duplicate->setEnabled(false);
                QMessageBox::information(this, "알림", "해당 아이디는 이용이 가능합니다!");
                ui->inputId->setEnabled(false);
            }else if(message=="fail"){
                QMessageBox::warning(this, "알림", " 중복된 아이디는 사용할수 없습니다. ");
                ui->inputId->clear();
            }
        }
        else if(signal=="sign_up_result")  // 결과가 success or fail
        {
            QString result = json["result"].toString();
            qDebug()<< result << "\n";
            if(result =="success"){
                qDebug()<< "가입 성공! \n";
                QMessageBox::information(this, "알림", "가입 완료되었습니다 !");
            }else{
                qDebug()<< "가입 실패! \n";
                QMessageBox::warning(this, "알림", "가입 실패되었습니다 !");
            }
        }

        //로그인으로 리다이렉션
        this->go_to_login_screen();
    });





    //생성자
}

void sign_up::do_check_duplicate(){


    // 아이디를 갖고 서버로 쏜다.
    QJsonObject req;
    req["signal"] = "check_id";
    req["role"] = "store";
    req["id"] = ui->inputId->text();
    client->sendJson(req);
    qDebug()<<"아이디 중복확인을 서버로 보냈습니다.";

}

void sign_up::do_store_signup(){
    //2 회원 가입 하는 함수 에서 json 전송 함수 호출
    // this->transaction_signup_function();
    QJsonObject req;
    req["signal"]= "sign_up";
    req["role"]="store";
    req["id"] = ui->inputId->text();
    req["pass"] = ui->pass_le->text();
    req["business_number"] = ui->bussiness_number_le->text();
    req["owner_name"] = ui->owner_name_le->text();
    req["owner_phone"] = ui->owner_phone_le->text();
    // ===========================================================
    // 영업점 정보
    req["store_name"] = ui->store_name_le->text();
    req["store_addr"] = ui->store_addr_le->text();
    req["store_phone"] = ui->store_phone_le->text();
    req["store_intro"] = ui->store_intro_le->text();
    req["open_time"] = ui->open_time_edit->text();
    req["close_time"] = ui->close_time_edit->text();
    req["min_order_le"] = ui->min_order_le->text();
    req["deliver_areas"] = ui->deliver_area_le->text();
    req["origin_info"] = ui->origin_info_le->text();
    req["nutritional_info"] = ui->nutritional_info_le->text();
    req["allergy_info"] = ui->allergy_le->text();


    client->sendJson(req);
    qDebug()<<"회원가입 요청을 서버로 보냈습니다.";
    qDebug()<<req <<"\n";
    //3 모든 입력 필드들 초기화 하는 함수 호출
    this->reset_input_fields();

    //4 로그인화면으로 이동 하는 함수 호출
    this->go_to_login_screen();

    //5 응답결과 response  결과를
}

void sign_up::reset_input_fields(){

    ui->inputId->setText("");
    ui->pass_le->setText("");
    ui->bussiness_number_le->setText("");
    ui->owner_name_le->setText("");
    ui->owner_phone_le->setText("");

    ui->store_name_le->setText("");
    ui->store_addr_le->setText("");
    ui->store_phone_le->setText("");
    ui->store_intro_le->setText("");
    ui->open_time_edit->setTime(QTime(0, 0)); // 0으로 초기화
    ui->close_time_edit->setTime(QTime::currentTime()); // 현재 시간으로 초기화
    ui->min_order_le->setText("");
    ui->deliver_area_le->setText("");
    ui->origin_info_le->setText("");
    ui->nutritional_info_le->setText("");
    ui->allergy_le->setText("");

}

void sign_up::go_to_login_screen(){
    emit req_turn_to_login();
}


void sign_up::setRegion(QMap<QString, QStringList>& regionMap){
    regionMap["서울특별시"] = {"강남구", "강동구", "강북구", "강서구", "관악구", "광진구",
                               "구로구", "금천구", "노원구", "도봉구", "동대문구", "동작구",
                               "마포구", "서대문구", "서초구", "성동구", "성북구", "송파구",
                               "양천구", "영등포구", "용산구", "은평구", "종로구", "중구", "중랑구"};
    regionMap["부산광역시"] = {"강서구", "금정구", "기장군", "남구", "동구", "동래구",
                               "부산진구", "북구", "사상구", "사하구", "서구", "수영구",
                               "연제구", "영도구", "중구", "해운대구"};
    regionMap["대구광역시"] = {"남구", "달서구", "달성군", "동구", "북구", "서구", "수성구", "중구"};
    regionMap["인천광역시"] = {"강화군", "계양구", "남동구", "동구", "미추홀구", "부평구",
                               "서구", "연수구", "옹진군", "중구"};
    regionMap["광주광역시"] = {"광산구", "남구", "동구", "북구", "서구"};
    regionMap["대전광역시"] = {"대덕구", "동구", "서구", "유성구", "중구"};
    regionMap["울산광역시"] = {"남구", "동구", "북구", "울주군", "중구"};
    regionMap["세종특별자치시"] = {"조치원읍", "한솔동", "도담동", "아름동", "종촌동", "고운동", "보람동", "새롬동", "다정동", "대평동", "소담동", "반곡동", "금남면", "부강면"};
    regionMap["경기도"] = {"가평군", "고양시", "과천시", "광명시", "광주시", "구리시", "군포시",
                           "김포시", "남양주시", "동두천시", "부천시", "성남시", "수원시",
                           "시흥시", "안산시", "안성시", "안양시", "양주시", "양평군", "여주시",
                           "연천군", "오산시", "용인시", "의왕시", "의정부시", "이천시", "파주시",
                           "평택시", "포천시", "하남시", "화성시"};
    regionMap["강원특별자치도"] = {"강릉시", "고성군", "동해시", "삼척시", "속초시", "양구군",
                                   "양양군", "영월군", "원주시", "인제군", "정선군", "철원군",
                                   "춘천시", "태백시", "평창군", "홍천군", "화천군", "횡성군"};
    regionMap["충청북도"] = {"괴산군", "단양군", "보은군", "영동군", "옥천군", "음성군", "제천시",
                             "진천군", "청주시", "충주시"};
    regionMap["충청남도"] = {"계룡시", "공주시", "금산군", "논산시", "당진시", "보령시", "부여군",
                             "서산시", "서천군", "아산시", "예산군", "천안시", "청양군", "태안군", "홍성군"};
    regionMap["전북특별자치도"] = {"고창군", "군산시", "김제시", "남원시", "무주군", "부안군", "순창군",
                                   "완주군", "익산시", "임실군", "장수군", "전주시", "정읍시", "진안군"};
    regionMap["전라남도"] = {"강진군", "고흥군", "곡성군", "광양시", "구례군", "나주시", "담양군",
                             "목포시", "무안군", "보성군", "순천시", "신안군", "여수시", "영광군",
                             "영암군", "완도군", "장성군", "장흥군", "진도군", "함평군", "해남군", "화순군"};
    regionMap["경상북도"] = {"경산시", "경주시", "고령군", "구미시", "군위군", "김천시", "문경시",
                             "봉화군", "상주시", "성주군", "안동시", "영덕군", "영양군", "영주시",
                             "영천시", "예천군", "울릉군", "울진군", "의성군", "청도군", "청송군",
                             "칠곡군", "포항시"};
    regionMap["경상남도"] = {"거제시", "거창군", "고성군", "김해시", "남해군", "밀양시", "사천시",
                             "산청군", "양산시", "의령군", "진주시", "창녕군", "창원시", "통영시",
                             "하동군", "함안군", "함양군", "합천군"};
    regionMap["제주특별자치도"] = {"서귀포시", "제주시"};
}

sign_up::~sign_up()
{
    delete ui;
}
