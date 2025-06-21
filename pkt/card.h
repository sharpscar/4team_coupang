#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QMessageBox>

// struct Order_items {
//     QString menu_name;
//     QString quantity;
//     QString option;
//     QString price;
// };

struct Orders_card{
    QString order_serialNumber;
    QString order_time;
    QString total_price;
    QString menu_quentity;
    QString order_items_menu_names;
};

struct OrderCardState{
    Orders_card order;
    bool is_rider_info_ready = false;
    bool is_complete_ready = false;
    QString rider_info;
};

struct RiderInfo{
    QString text;
};

namespace Ui {
class Card;
}

class Card : public QWidget
{
    Q_OBJECT

public:
    explicit Card(QWidget *parent = nullptr);
     void setText(const QString& text);
    // Orders m_orders;
    ~Card();

    void set_rider_Text(const QString &text);
    void set_complete_Text(const QString &text);
signals:
    void clicked();
    void doneButtonClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void updateStyle(bool hovered);
    QPushButton* doneButton;
    QPushButton* riderInfoButton;
    QPushButton* completeButton;
    RiderInfo riderInfo_;

    Ui::Card *ui;

};

#endif // CARD_H
