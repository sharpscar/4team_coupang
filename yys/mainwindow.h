#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "tcpclient.h"
#include <QMainWindow>
#include <QDialog>
#include <QTimer>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

#include <QGesture>
#include <QGestureEvent>
#include <QScrollBar>
#include <QScroller>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>

struct rider_receive
{
    QString store_name; //가게 명
    QString store_addr; //가게 주소

    QString order_num;   //주문번호
    QString to_rider;   //요청사항
    QString total_price;//총 금액

    QString user_addr;  //유저 주소
    QString user_pnum;  //유저 번호

};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QVector<rider_receive> rider_order_list;
    QString message;
    QString store_name;
    QString order_num;


    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TcpClient* client;
    rider_receive currentrider;
};
#endif // MAINWINDOW_H
