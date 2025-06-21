#include "orderresdialog.h"
#include "ui_orderresdialog.h"

OrderResDialog::OrderResDialog(Orders &orders_,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrderResDialog),orders_(orders_)

{
    ui->setupUi(this);


    int rowCount = orders_.order_Items.size();
    int columnCount  =4;

    QStandardItemModel* model = new QStandardItemModel(rowCount, columnCount, this);
    model->setHorizontalHeaderLabels({"메뉴명", "수량", "가격", "옵션"});

    // 너비를 맞춘다.
    ui->tv_dialog->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    qDebug()<<"rowCount!"<<rowCount;
    qDebug() << "order_Items size: " << orders_.order_Items.size();

    // 행 , 열


    model->setItem(0, 0, new QStandardItem(orders_.order_Items.last().menu_name));
    model->setItem(0, 1, new QStandardItem(orders_.order_Items.last().quantity));
    model->setItem(0, 2, new QStandardItem(orders_.order_Items.last().price));
    model->setItem(0, 3, new QStandardItem(orders_.order_Items.last().option));

    ui->tv_dialog->setModel(model);



}

OrderResDialog::~OrderResDialog()
{
    // delete ui;

    // connect(ui->btn10min, &QPushButton::clicked, this, &OrderResDialog::on_btn10min_clicked);
    // connect(ui->btnReject, &QPushButton::clicked, this, &OrderResDialog::on_btn20min_clicked);
    // connect(ui->btn30min, &QPushButton::clicked, this, &OrderResDialog::on_btn30min_clicked);
    // connect(ui->btn20min, &QPushButton::clicked, this, &OrderResDialog::on_btnReject_clicked);

}

void OrderResDialog::on_btn10min_clicked()
{
    responseTime = 10;
    accept();  // 다이얼로그 종료
}

void OrderResDialog::on_btn20min_clicked()
{
    responseTime = 20;
    accept();
}

void OrderResDialog::on_btn30min_clicked()
{
    responseTime = 30;
    accept();
}

void OrderResDialog::on_btnReject_clicked()
{
    responseTime = -1; // 거절은 -1로 표시
    accept();
}

int OrderResDialog::getResponseTime() const
{
    return responseTime;
}
