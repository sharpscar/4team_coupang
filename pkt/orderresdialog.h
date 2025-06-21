#ifndef ORDERRESDIALOG_H
#define ORDERRESDIALOG_H

#include <QDialog>
#include "ordercheck.h"
#include <QStandardItemModel>


namespace Ui {
class OrderResDialog;
}

class OrderResDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderResDialog( Orders& orders_, QWidget *parent = nullptr);
    ~OrderResDialog();
    int getResponseTime() const;
private slots:


    void on_btn10min_clicked();
    void on_btn20min_clicked();
    void on_btn30min_clicked();
    void on_btnReject_clicked();

private:
    Ui::OrderResDialog *ui;
    int responseTime;
    Orders& orders_;

};

#endif // ORDERRESDIALOG_H
