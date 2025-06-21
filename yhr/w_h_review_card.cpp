#include "w_h_review_card.h"
#include "ui_w_h_review_card.h"

w_h_review_card::w_h_review_card(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::w_h_review_card)
{
    ui->setupUi(this);
}

w_h_review_card::~w_h_review_card()
{
    delete ui;
}
