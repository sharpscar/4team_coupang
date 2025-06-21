#include "part3_search_page.h"
#include "ui_part3_search_page.h"

part3_search_page::part3_search_page(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::part3_search_page)
{
    ui->setupUi(this);
}

part3_search_page::~part3_search_page()
{
    delete ui;
}
