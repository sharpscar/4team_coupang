#include "menumanage.h"
#include "ui_menumanage.h"

#include <QMessageBox>
#include <QSplitter>



MenuManage::MenuManage(TcpClient * client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuManage), client(client)
{
    ui->setupUi(this);

    //홈을 누르면 대시보드로 이동 이동!
    connect(ui->btn_home, &QPushButton::clicked, this, &MenuManage::go_to_dash_screen);

    //주문을 누르면 주문확인으로 이동 이동!
    connect(ui->btn_order, &QPushButton::clicked, this, &MenuManage::go_to_ordercheck_screen);

    //영업시간을 누르면 메뉴관리로 이동 이동!
    connect(ui->btn_time, &QPushButton::clicked, this, &MenuManage::go_to_timeManage_screen);

    ui->menu_tw->setRowCount(2);
    ui->menu_tw->setColumnCount(3);



    //3단 구성된 레이아웃 크기를 변경 가능한 스플리터를 설정
    QSplitter * splitter = new QSplitter(Qt::Vertical);

    splitter->addWidget(ui->frame_1);
    splitter->addWidget(ui->menu_tw);
    splitter->addWidget(ui->frame_2);
    ui->gridLayout_3->addWidget(splitter);


    // 메뉴 추가 버튼
    connect(ui->btn_add, &QPushButton::clicked, this, &MenuManage::addMenu);

    //수정을 위해 테이블위젯을 클릭하면 폼에 값들을 넣는다.
    connect(ui->menu_tw, &QTableWidget::cellClicked, this, &MenuManage::onTableItemClicked);

    // 메뉴 수정버튼
    connect(ui->btn_update, &QPushButton::clicked, this, &MenuManage::updateMenu);

    // 메뉴 삭제버튼z
    connect(ui->btn_del, &QPushButton::clicked, this, &MenuManage::deleteMenu);

    ui->cb_menu_status->addItem("ACTIVE");
    ui->cb_menu_status->addItem("DEACTIVE");
}

// 메뉴를 삭제하는 기능
void MenuManage::deleteMenu(){

    // 아무것도 선택 안하고 삭제버튼을 누른경우
    if(row_index <0){
        QMessageBox::warning(this, "안된당","응? 일단 테이블을 선택해야 어떤걸 삭제할지 알수 있지 않을까?");
        return;
    }

    //정말 삭제할지 물어보는 ui
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "삭제확인", "정말 삭제하시겠습니까?", QMessageBox::Yes | QMessageBox::No);

    // yes를 눌렀을때
    if(reply==QMessageBox::Yes){
        if(row_index >= 0 && row_index< menuList.size()){
            menuList.removeAt(row_index);
        }
        ui->menu_tw->removeRow(row_index);
    }



}


void MenuManage::go_to_dash_screen(){
    emit  req_turn_to_dashboard();
}

// 메뉴 추가함수 storeInfo의 menuInfo의 menu_category
void MenuManage::addMenu(){

    MenuInfo new_menu;
    new_menu.menu_name = ui->name_le->text();
    new_menu.menu_price = ui->price_le->text();
    new_menu.menu_category = ui->cb_cate->currentText();
    new_menu.menu_status = ui->cb_menu_status->currentText();
    new_menu.menu_info = ui->desc_le->text();
    si.menus.append(new_menu);

    // 생성한 json 서버로 송신하는 코드 필요
    this->initialize_screen();

}


// 메뉴 수정함수
void MenuManage::updateMenu(){

    MenuInfo updateMenu;
    updateMenu.menu_name = ui->name_le->text();
    updateMenu.menu_price = ui->price_le->text();
    updateMenu.menu_info = ui->desc_le->text();
    updateMenu.menu_category = ui->cb_cate->currentText();
    updateMenu.menu_status = ui->cb_menu_status->currentText();


    //유효성 검증 배열 크기
    if(row_index <0 || row_index>= si.menus.size())
    {
        qDebug()<<"삐빅! -- 인덱스 문제 99라인 봐라";
    }

    si.menus[row_index]= updateMenu;
    this->initialize_screen();
}



//로그인 되면 무조건 실행되기때문에 가게정보가 무조건 설정된다고 상정하자
void MenuManage::set_store_info(StoreInfo &store){
    this->si = store;


    //25.0616 메뉴 정보 건드릴때 여기서 이제 위젯들을 initializing 해야한다.
    this->initialize_screen();
}

// 화면을 초기화하는 함수
void MenuManage::initialize_screen(){    

    //si = store_info -가게정보
    menuList = si.menus;
    all_menu_count = si.menus.size();

    menu_active_state = std::count_if(menuList.begin(), menuList.end(), [](const MenuInfo &m){
        return m.menu_status.toUpper() == "ACTIVE";
    });

    menu_deactive_state = std::count_if(menuList.begin(), menuList.end(), [](const MenuInfo& m){
        return m.menu_status.toUpper() == "DEACTIVE" ;
    });

    // 화면 상단의 현재 판매중인 내역
    ui->lb_all_menu_count->setText(QString::number(all_menu_count));
    ui->lb_menu_active_state->setText(QString::number(menu_active_state));
    ui->lb_menu_deactive_state->setText(QString::number(menu_deactive_state));

    ui->menu_tw->setRowCount(all_menu_count);
    ui->menu_tw->setColumnCount(5);

    QStringList headers = {"카테고리","메뉴명","메뉴 설명","가격","메뉴 상태"};
    ui->menu_tw->setHorizontalHeaderLabels(headers);
    for (int i =0; i<all_menu_count; i++){

        ui->menu_tw->setItem(i,0, new QTableWidgetItem(menuList[i].menu_category));
        ui->menu_tw->setItem(i,1, new QTableWidgetItem(menuList[i].menu_name));
        ui->menu_tw->setItem(i,2, new QTableWidgetItem(menuList[i].menu_info));
        ui->menu_tw->setItem(i,3, new QTableWidgetItem(menuList[i].menu_price));
        ui->menu_tw->setItem(i,4, new QTableWidgetItem(menuList[i].menu_status));

    }

    // 예: 열 비율을 3:2:2:3으로 설정

    ui->menu_tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //메뉴 카테고리리 콤보박스 세팅
    makeCategoryComboboxSetting();
    //메뉴 상태 콤보박스 세팅

}



void MenuManage::onTableItemClicked(int row, int column){

    row_index = row;
    QString selectedCategory = ui->menu_tw->item(row, 0)->text();
    QString selectedState = ui->menu_tw->item(row, 4)->text();
    int state_index = ui->cb_menu_status->findText(selectedState);

    int category_index = ui->cb_cate->findText(selectedCategory);
    if(category_index != -1){
        ui->cb_cate->setCurrentIndex(category_index);
    }
    if(state_index != -1){
        ui->cb_menu_status->setCurrentIndex(state_index);
    }

    ui->name_le->setText(menuList[row].menu_name);
    ui->price_le->setText(menuList[row].menu_price);
    ui->desc_le->setText(menuList[row].menu_info);



    //int QComboBox::findText(const QString &text, Qt::MatchFlags flags = Qt::MatchExactly) const << 이건 이미 정의되어 있다고 한다.
}



void MenuManage::makeCategoryComboboxSetting(){
    QSet<QString> categorySet;
    QList menuList = this->si.menus;

    // qDebug()<<"makeCategoryComboboxSetting함수 내부 ";
    // qDebug()<<"메뉴 갯수 "<<  menuList.size();

    for (int i=0; i< menuList.size(); i++){
        QString category = menuList[i].menu_category;
        categorySet.insert(category);
    }

    ui->cb_cate->clear();
    ui->cb_cate->addItem("..");

    for(const QString &category: categorySet){
        ui->cb_cate->addItem(category);
    }
}


void MenuManage::go_to_ordercheck_screen(){
    emit req_turn_to_ordercheck();
}

void MenuManage::go_to_timeManage_screen(){
    emit req_turn_to_timeManage();
}

MenuManage::~MenuManage()
{
    delete ui;
}
