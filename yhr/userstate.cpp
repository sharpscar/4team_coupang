#include "userstate.h"

// 생성 시 로그인 여부는 false 여야 함
UserState::UserState(QObject *parent) : QObject(parent), isLoggedIn(false) {}

// 내가 만든 쿠팡이츠 앱에서 USerState 객체를 전역에서 하나만 사용하고 싶을 때 쓰는 방법
UserState& UserState::getInstance() {
    static UserState instance;
    return instance;
}

void UserState:: login(const QString& name, const QString& email, const QString& phone) {
    isLoggedIn = true;
    userName = name;
    this->email = email;
    phoneNumber = phone;
    emit loginStatusChanged(true);
}

void UserState::logout() {
    emit log_out();
}

void UserState::addAddress(const AddressInfo& address){
    addresses.append(address);
    emit addressChanged();
}

void UserState::changePrimaryAddress(int index) {
    for (int i = 0; i < addresses.size(); ++i) {
        addresses[i].isPrimary = (i == index);
    }
    emit addressChanged();
}

void UserState::addToCart(const CartItem& item){
    cart.items.append(item);
    emit cartUpdated();
}

void UserState::clearCart() {
    cart = CartInfo();
    emit orderComplete();
}
