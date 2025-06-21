#ifndef W_H_REVIEW_CARD_H
#define W_H_REVIEW_CARD_H

#include <QWidget>

namespace Ui {
class w_h_review_card;
}

class w_h_review_card : public QWidget
{
    Q_OBJECT

public:
    explicit w_h_review_card(QWidget *parent = nullptr);
    ~w_h_review_card();

private:
    Ui::w_h_review_card *ui;
};

#endif // W_H_REVIEW_CARD_H
