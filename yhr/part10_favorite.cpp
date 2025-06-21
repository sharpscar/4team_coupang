#include "part10_favorite.h"
#include "ui_part10_favorite.h"

part10_favorite::part10_favorite(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::part10_favorite)
{
    ui->setupUi(this);

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


    connect(ui->BtnHome, &QPushButton::clicked, this, [=](){
        emit reqTurnToMain();
    });

}

part10_favorite::~part10_favorite()
{
    delete ui;
}
