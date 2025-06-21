#ifndef PART9_WRITE_REVIEW_H
#define PART9_WRITE_REVIEW_H

#include "tcpclient.h"
#include "userstate.h"

#include <QWidget>
#include <QPushButton>
#include <QVector>
#include <QString>
#include <QScroller>


namespace Ui {
class part9_write_review;
}

class part9_write_review : public QWidget
{
    Q_OBJECT

public:
    explicit part9_write_review(TcpClient *client, QWidget *parent = nullptr);
    ~part9_write_review();

signals:
    void reqTurnToMain();

private:
    Ui::part9_write_review *ui;
    TcpClient *client;


};

#endif // PART9_WRITE_REVIEW_H
