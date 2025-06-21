#ifndef PART3_SEARCH_PAGE_H
#define PART3_SEARCH_PAGE_H

#include <QWidget>

namespace Ui {
class part3_search_page;
}

class part3_search_page : public QWidget
{
    Q_OBJECT

public:
    explicit part3_search_page(QWidget *parent = nullptr);
    ~part3_search_page();

private:
    Ui::part3_search_page *ui;
};

#endif // PART3_SEARCH_PAGE_H
