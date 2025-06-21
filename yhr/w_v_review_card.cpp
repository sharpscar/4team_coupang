#include "w_v_review_card.h"
#include "ui_w_v_review_card.h"

w_v_review_card::w_v_review_card(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::w_v_review_card)
{
    ui->setupUi(this);

    ui->label_comment->setWordWrap(true);
}

w_v_review_card::~w_v_review_card()
{
    delete ui;
}
