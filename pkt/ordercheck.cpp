#include "ordercheck.h"
#include "ui_ordercheck.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

Ordercheck::Ordercheck(TcpClient *client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Ordercheck),client(client)
{
    ui->setupUi(this);



    connect(ui->btn_home, &QPushButton::clicked, this, &Ordercheck::go_to_dash_screen);

    //주문을 누르면 주문확인으로 이동 이동!
    connect(ui->btn_order, &QPushButton::clicked, this, &Ordercheck::go_to_ordercheck_screen);

    //영업시간을 누르면 영업 시간 이동 이동!
    connect(ui->btn_time, &QPushButton::clicked, this, &Ordercheck::go_to_timeManage_screen);

    //메뉴관리 누르면 메뉴관리로 이동
    connect(ui->btn_menu,&QPushButton::clicked, this, &Ordercheck::go_to_menuManage_screen);




    ui->order_detail_tw->setRowCount(8);
    ui->order_detail_tw->setColumnCount(4);

    QStringList headers = {"메뉴","수량","금액","옵션"};
    ui->order_detail_tw->setHorizontalHeaderLabels(headers);


    ui->order_detail_tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



}


void Ordercheck::saveJsonToFile(const QJsonObject& json) {
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
\

// 라이더 정보가 오면  카드를 찾아서  해당 카드에 정보를 세팅
void Ordercheck::onRiderInfoReceived(const QString& order_serialNumber, const QString& rider_info){
    Card* targetCard = m_cards.value(order_serialNumber);
    if(targetCard) {
        targetCard->set_rider_Text(rider_info);
    }
}



void Ordercheck::onCompleteInfoReceived(const QString& order_serialNumber){
    Card* targetCard = m_cards.value(order_serialNumber);
    if(targetCard) {
        targetCard->set_complete_Text(order_serialNumber);
    }
}

void Ordercheck::onDoneButtonClicked(QString sn){
    qDebug()<<"시그널 번호 :"<<sn;
    QJsonObject req;
    req["signal"]= "req_rider_info";
    req["role"] = "store";
    req["store_name"] = "교촌치킨";
    req["order_serialNumber"]=sn;
    client->sendJson(req);

    emit req_turn_to_ordercheck();

}
void Ordercheck::go_to_dash_screen(){
    emit req_turn_to_dashboard();
}

void Ordercheck::go_to_ordercheck_screen(){
    emit req_turn_to_ordercheck();
}

void Ordercheck::go_to_timeManage_screen(){
    emit req_turn_to_timeManage();
}

// 메뉴체크
void Ordercheck::go_to_menuManage_screen(){
    emit req_turn_to_menuManage();
}

//로그인시에 주문정보를 셋팅
void Ordercheck::change_order_info(QVector<Orders> &allOrders_){
    this->allOrders = allOrders_;

    qDebug()<< "ordercheck 291line :change_order_info함수 : 오더 일련번호 "<<allOrders.first().order_serialNumber;
}



//로그인 되면 무조건 실행되기때문에 가게정보가 무조건 설정된다고 상정하자
void Ordercheck::add_order_info(Orders &orders){


    this->allOrders.append(orders);
    this->setCards();

    //for문 끝


}

void Ordercheck::setCards(){


    //이제 실제 오더가 들어오면 여기서 카드를 생성하게 될것이다 그전까진 가짜데이터

    ui->on_place_cards_layout->setAlignment(Qt::AlignTop);

    QLayoutItem* child;
    // while ((child = ui->on_place_cards_layout->takeAt(0)) != nullptr) {
    //     if (child->widget()) child->widget()->deleteLater();
    //     delete child;

    // }

    // double free를 막는 코드 --> 이미 없는 카드를 map에서 찾아서 프로그램 크래시 발생함.
    while((child = ui->on_place_cards_layout->takeAt(0))!=nullptr){
        if(child->widget()){
            for(auto it = m_cards.begin(); it != m_cards.end();){
                if(it.value()==child->widget()){
                    it = m_cards.erase(it);
                }else{
                    ++it;
                }
            }
            child->widget()->deleteLater();
        }
        delete child;
    }

    // BtnBYWidget 동적 추가
    for (const Orders& orders : allOrders) {


        // 카드가 존재하면 새로 생성하지 않음 이미 있는거 쓴다.

        Card* card = m_cards.value(orders.order_serialNumber);
        qDebug()<< "allOrders를 참조하여 각 주문에 대해 시리얼번호를 참조하고 맵카드에 있는걸 참조한다." <<orders.order_serialNumber;
        if(card){
            qDebug()<<"(ꔷ̥̑.̮ꔷ̥̑)       카드가 이미 존재합니다.";
            QString myText = QString("%1  \n %2  %3 \n  %4 \n  %5").arg(orders.order_serialNumber,orders.order_time, orders.menu_quentity, orders.total_price, orders.order_items_menu_names);
            card->setText(myText);



            QString serialCopy = orders.order_serialNumber;

            // card 조리완료 버튼 에서 보낸 시그널을 여기서 포착
            connect(card, &Card::doneButtonClicked, this,  [serialCopy ,this](){

                //조리완료 버튼이 눌렸다 (주문번호)
                onDoneButtonClicked(serialCopy);
            });
        }else{

            // 카드가 존재 하지 않으면 새로생성
            qDebug()<<"(ꔷ̥̑.̮ꔷ̥̑)       카드를 새로 생성합니다.";

            card = new Card( this);
            m_cards[orders.order_serialNumber] = card;

            QString time_part2 = orders.order_time.split(" ").at(1);

            QString hourMin = time_part2.left(5);

            QString myText = QString("%1  \n %2 \n %3 \n %4 %5").arg("주문번호 :"+orders.order_serialNumber,"주문시간 : "+hourMin,"메뉴명 : "+ orders.order_items_menu_names, "주문총액 : "+orders.total_price+"원","수량 : "+orders.menu_quentity );
            card->setText(myText);
            QString serialCopy = orders.order_serialNumber;
            // card 조리완료 버튼 에서 보낸 시그널을 여기서 포착
            connect(card, &Card::doneButtonClicked, this,  [serialCopy, this](){

                //시리얼번호 넘겨주는
                onDoneButtonClicked(serialCopy);
            });
        }
         ui->on_place_cards_layout->addWidget(card);


        //카드에 클릭 이벤트를 걸자
        connect(card, &Card::clicked, this, [=](){
            bool ok;
            int rowCount = orders.menu_quentity.toInt(&ok);
            // qDebug()<< "에밋 된거 들었다. ";
            if(!ok){
                qDebug()<<"변환 실패";
            }


            qDebug()<<"rowCount"<<rowCount;
            //메뉴의 수만큼 로우 갯수를 만듬
            ui->order_detail_tw->setRowCount(rowCount);

            ui->order_serialNumber->setText(orders.order_serialNumber);
            QString order_time = orders.order_time;
            ui->lb_order_time->setText(orders.order_time.split(" ").at(1));

            for (int i=0; i< rowCount; i++){

                qDebug()<<"rowCount"<<rowCount;

                ui->order_detail_tw->setItem(i, 0, new QTableWidgetItem(orders.order_Items[i].menu_name));
                ui->order_detail_tw->setItem(i, 1, new QTableWidgetItem(orders.order_Items[i].quantity));
                ui->order_detail_tw->setItem(i, 2, new QTableWidgetItem(orders.order_Items[i].price));
                ui->order_detail_tw->setItem(i, 3, new QTableWidgetItem(orders.order_Items[i].option));

            }


        });
    }
}


Ordercheck::~Ordercheck()
{
    delete ui;
}
