#ifndef PART3_SEARCH_H
#define PART3_SEARCH_H

#include <QWidget>

namespace Ui {
class part3_search;
}

class part3_search : public QWidget
{
    Q_OBJECT

public:
    explicit part3_search(QWidget *parent = nullptr);
    ~part3_search();

signals:
    void reqTurnToMain();

private:
    Ui::part3_search *ui;
};

#endif // PART3_SEARCH_H
