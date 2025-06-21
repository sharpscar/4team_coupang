#ifndef W_H_CATEGORY_H
#define W_H_CATEGORY_H

#include <QWidget>
#include <QStringList>
#include <QToolButton>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QScroller>
const int SIZE_ = 66;

namespace Ui {
class w_h_category;
}

class w_h_category : public QWidget
{
    Q_OBJECT

public:
    explicit w_h_category(QWidget *parent = nullptr);
    QMap<QString, QString> categoryImageMap;
    QStringList categories;
    ~w_h_category();

private:
    Ui::w_h_category *ui;
};

#endif // W_H_CATEGORY_H
