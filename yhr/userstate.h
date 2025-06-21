#ifndef USERSTATE_H
#define USERSTATE_H

#include "dataType.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QLabel>
#include <QTimer>

class UserState : public QObject {
    Q_OBJECT

public:
    explicit UserState(QObject *parent = nullptr);
    static UserState& getInstance();

    // 로그인 관련 정보
    bool isLoggedIn;
    QString userName;
    QString email;
    QString phoneNumber;

    // 주소 정보 (여러 개 가능)
    QVector<AddressInfo> addresses;
    int selectedAddressIndex = -1;

    // 장바구니 정보 (한 가게만 가능)
    CartInfo cart;

    // 메서드
    void login(const QString& name, const QString& email, const QString& phone);
    void logout() ;

    // 액션 후 간략한 메세지 출력
    void showMSG(const QString& message, int ms);
    // 주소 추가
    void addAddress(const AddressInfo& address);
    // 기본 주소지 변경
    void changePrimaryAddress(int index);
    // 주소지 존재 여부
    bool hasAddress();

    // 한 가게의 장바구니 메뉴만 담을 수 있으니 확인
    void addToCart(const CartItem& item);
    // 장바구니 삭제
    void clearCart();

signals:
    void loginStatusChanged(bool isloggedIn);
    void addressChanged();
    void cartUpdated();
    void orderComplete();
    void log_out();

};

#endif // USERSTATE_H
