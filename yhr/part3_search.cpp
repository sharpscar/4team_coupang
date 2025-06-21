#include "part3_search.h"
#include "ui_part3_search.h"

part3_search::part3_search(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::part3_search)
{
    ui->setupUi(this);

    connect(ui->btnBackToMain, &QPushButton::clicked, this, [=](){
        emit reqTurnToMain();
    });

    connect(ui->BtnHome, &QPushButton::clicked,this, [=](){
        emit reqTurnToMain();
    });
}

part3_search::~part3_search()
{
    delete ui;
}
