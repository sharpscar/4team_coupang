#ifndef W_V_MENU_CARD_H
#define W_V_MENU_CARD_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>

namespace Ui {
class w_v_menu_card;
}

class w_v_menu_card : public QWidget
{
    Q_OBJECT

public:
    explicit w_v_menu_card(QWidget *parent = nullptr);
    ~w_v_menu_card();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void clicked();

private:
    Ui::w_v_menu_card *ui;
    void updateStyle(bool hovered);
};

#endif // W_V_MENU_CARD_H



