#include "timemanage.h"
#include "ui_timemanage.h"
#include <QMessageBox>

TimeManage::TimeManage(TcpClient * client,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimeManage)
{
    ui->setupUi(this);

    //홈을 누르면 대시보드로 이동 이동!
    connect(ui->btn_home, &QPushButton::clicked, this, &TimeManage::go_to_dash_screen);

    //주문을 누르면 주문확인으로 이동 이동!
    connect(ui->btn_order, &QPushButton::clicked, this, &TimeManage::go_to_ordercheck_screen);

    //영업시간을 누르면 영업시간 관리로 이동 이동!
    connect(ui->btn_time, &QPushButton::clicked, this, &TimeManage::go_to_timeManage_screen);

    //메뉴관리 누르면 메뉴관리로 이동 이동!
    connect(ui->btn_menu, &QPushButton::clicked, this, &TimeManage::go_to_menuManage_screen);

    connect(ui->pushButton_2, &QPushButton::clicked, this, [=, this](){
        QMessageBox::information(this,"영업시간 설정","영업시간이 설정되었습니다.");
    });


    // 테이블위젯 생성 (이미 UI에 있다면 ui->tableWidget으로 접근)
    ui->tableWidget->setRowCount(31);  // 31개 행
    ui->tableWidget->setColumnCount(4);  // 4개 컬럼

    // 헤더 이름 설정


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 오늘
    QDate today = QDate::currentDate();
    ui->tableWidget->setRowCount(15);
    ui->tableWidget->setColumnCount(3);


    QStringList headers;
    headers << "날짜" << "요일" << "오픈/마감시간";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for(int i=0; i<15; i++){
        QLocale korean(QLocale::Korean, QLocale::SouthKorea);
        //오늘날짜
        QDate targetDate = today.addDays(i);
        //날짜 문자열 형태
        QString dateStr = targetDate.toString("yyyy-MM-dd");
        //요일 형태

        QString dayOfWeekStr = korean.toString(targetDate,"dddd");

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(dateStr));

        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(dayOfWeekStr));

        //날짜/요일
        QWidget *timeWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(timeWidget);
        layout->setContentsMargins(0,0,0,0);



        QTimeEdit *openTimeEdit = new QTimeEdit();
        QTimeEdit *closeTimeEdit = new QTimeEdit();

        openTimeEdit->setTime(QTime(12,0));
        closeTimeEdit->setTime(QTime(23,0));
        layout->addWidget(openTimeEdit);
        layout->addWidget(closeTimeEdit);

        ui->tableWidget->setCellWidget(i,2,timeWidget);


    }

    // 오픈/마감 시간 셀에 위젯 추가
    QWidget *timeWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(timeWidget);
    layout->setContentsMargins(0, 0, 0, 0);  // 여백 없애기

    QTimeEdit *openTimeEdit = new QTimeEdit();
    QTimeEdit *closeTimeEdit = new QTimeEdit();

    layout->addWidget(openTimeEdit);
    layout->addWidget(closeTimeEdit);

    ui->tableWidget->setCellWidget(0, 2, timeWidget);

    // 3:3:4 컬럼 비율 조정
    int totalWidth = ui->tableWidget->width();
    ui->tableWidget->setColumnWidth(0, totalWidth * 3 / 10);  // 날짜 30%
    ui->tableWidget->setColumnWidth(1, totalWidth * 3 / 10);  // 요일 30%
    ui->tableWidget->setColumnWidth(2, totalWidth * 4 / 10);  // 오픈/마감 40%
    ui->tableWidget->setColumnWidth(3, 80);  // 확인 버튼은 고정폭 (예: 80픽셀)




}


void TimeManage::go_to_dash_screen(){
    emit req_turn_to_dashboard();
}

void TimeManage::go_to_ordercheck_screen(){
    emit req_turn_to_ordercheck();
}

void TimeManage::go_to_timeManage_screen(){
    emit req_turn_to_timeManage();
}

void TimeManage::go_to_menuManage_screen(){
    emit req_turn_to_menuManage();
}


TimeManage::~TimeManage()
{
    delete ui;
}
