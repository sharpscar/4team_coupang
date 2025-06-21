#ifndef RIDER_MAKE_ID_H
#define RIDER_MAKE_ID_H

#include <QWidget>

namespace Ui {
class rider_make_id;
}

class rider_make_id : public QWidget
{
    Q_OBJECT

public:
    explicit rider_make_id(QWidget *parent = nullptr);
    ~rider_make_id();

private:
    Ui::rider_make_id *ui;
};

#endif // RIDER_MAKE_ID_H
