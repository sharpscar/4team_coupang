#ifndef PART1__FAVORITE_H
#define PART1__FAVORITE_H

#include <QWidget>

namespace Ui {
class part10_favorite;
}

class part10_favorite : public QWidget
{
    Q_OBJECT

public:
    explicit part10_favorite(QWidget *parent = nullptr);
    ~part10_favorite();

signals:
    void reqTurnToMain();

private:
    Ui::part10_favorite *ui;
};

#endif // PART1__FAVORITE_H
