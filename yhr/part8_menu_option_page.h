#ifndef PART8_MENU_OPTION_PAGE_H
#define PART8_MENU_OPTION_PAGE_H

#include "dataType.h"
#include "userstate.h"
#include "tcpclient.h"

#include <QWidget>
#include <QFrame>
#include <QLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QScrollBar>
#include <QScroller>

namespace Ui {
class part8_menu_option_page;
}

class part8_menu_option_page : public QWidget
{
    Q_OBJECT

public:
    explicit part8_menu_option_page(TcpClient* client, QString menu_name, TopData& data, QWidget *parent = nullptr);

    QString menu_name = "";
    MenuInfo* currentMenu = nullptr;
    QMap<QString, QFrame*> requiredFrames;
    QMap<QString, QLabel*> requiredTitles;

    void settingUI();
    MenuInfo* findMenuInTopData(const QString& targetName);
    void drawMenuOptions();
    bool validateRequiredOptions();
    void clearLayout(QLayout* layout);
    void handleScrollChanged(int value);


    ~part8_menu_option_page();

signals:
    void reqBackStorePage();

private:
    Ui::part8_menu_option_page *ui;
    TopData& data;
    TcpClient* client;
};

#endif // PART8_MENU_OPTION_PAGE_H
