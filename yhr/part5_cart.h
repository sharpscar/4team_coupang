#ifndef PART5_CART_H
#define PART5_CART_H

#include <QWidget>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QJsonArray>
#include "tcpclient.h"
#include "userstate.h"
#include <QScroller>

namespace Ui {
class part5_cart;
}

class part5_cart : public QWidget
{
    Q_OBJECT

public:
    explicit part5_cart(TcpClient* client, QWidget *parent = nullptr);
    void sendOrderToserver();
    QString fullAddress;
    ~part5_cart();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void reqToMainPage();
    void sucessOrder();

private slots:
    void onDeliveryOptionToggled(QAbstractButton *button, bool checked);
    void selectDeliveryOption(int index);

private:
    Ui::part5_cart *ui;
    TcpClient* client;
    QButtonGroup *deliveryGroup;
};

#endif // PART5_CART_H
