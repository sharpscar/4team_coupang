#include "rider_make_id.h"
#include "ui_rider_make_id.h"

rider_make_id::rider_make_id(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::rider_make_id)
{
    ui->setupUi(this);

    ui->okay_btn->setEnabled(false);

    ui->back_to_login->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px;");

    ui->insert_pnum->setPlaceholderText("휴대폰 번호를 입력해주세요");
    ui->insert_pw->setPlaceholderText("비밀번호를 입력해주세요");
    ui->insert_name->setPlaceholderText("이름을 입력해주세요");
    ui->insert_PI->setPlaceholderText("주민등록 번호를 입력해주세요");
    ui->insert_addr->setPlaceholderText("주소를 입력해주세요");
    ui->insert_motor_num->setPlaceholderText("휴대폰 번호를 입력해주세요");
    ui->insert_local->setPlaceholderText("휴대폰 번호를 입력해주세요");



    //회원가입시 받는 정보들을 리스트로 모은다
    QList<QLineEdit*> makeid_list = {
    ui->insert_pnum, ui->insert_pw, ui->insert_name, ui->insert_PI, ui->insert_addr, ui->insert_local, ui->insert_motor_num};
    //리스트에 포함된 모든 line edit테두리 삭제
    for (QLineEdit* lineEdit : makeid_list) {
        lineEdit->setStyleSheet("border: none;");
    }
    //요구하는 모든 빈칸이 채워져있으면 회원가입 버튼 활성화
    for (QLineEdit* lineEdit : makeid_list) {
        connect(lineEdit, &QLineEdit::textChanged, this, [=]() {
            bool allFilled = true;
            for (QLineEdit* edit : makeid_list) {
                if (edit->text().isEmpty()) {
                    allFilled = false;
                    break;
                }
            }
            ui->okay_btn->setEnabled(allFilled);
        });
    }



}

rider_make_id::~rider_make_id()
{
    delete ui;
}
