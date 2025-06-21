#ifndef W_V_ADDR_CARD_H
#define W_V_ADDR_CARD_H

#include <QWidget>
#include <QMouseEvent>
#include <QEnterEvent>

namespace Ui {
class w_v_addr_card;
}

class w_v_addr_card : public QWidget
{
    Q_OBJECT

public:
    explicit w_v_addr_card(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
    void updateFrameStyle(bool hover);
    ~w_v_addr_card();

signals:
    void frameClicked();

private:
    Ui::w_v_addr_card *ui;
};

#endif // W_V_ADDR_CARD_H


