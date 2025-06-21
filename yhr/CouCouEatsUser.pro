QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    part10_favorite.cpp \
    part5_cart.cpp \
    part6_order_complete.cpp \
    part7_store_page.cpp \
    part8_menu_option_page.cpp \
    part9_write_review.cpp \
    userstate.cpp \
    w_grid_category.cpp \
    tcpclient.cpp \
    w_h_category.cpp \
    w_v_addr_card.cpp \
    w_v_menu_card.cpp \
    w_v_store_card.cpp \
    my_page.cpp \
    part2_address.cpp \
    part3_search.cpp \
    part4_clicked_cat_page.cpp \
    part1_login_signup.cpp \
    w_v_review_card.cpp

HEADERS += \
    mainwindow.h \
    part10_favorite.h \
    part5_cart.h \
    part6_order_complete.h \
    part7_store_page.h \
    part8_menu_option_page.h \
    part9_write_review.h \
    userstate.h \
    w_grid_category.h \
    tcpclient.h \
    w_h_category.h \
    w_v_addr_card.h \
    w_v_menu_card.h \
    w_v_store_card.h \
    dataType.h \
    my_page.h \
    part2_address.h \
    part3_search.h \
    part4_clicked_cat_page.h \
    part1_login_signup.h \
    w_v_review_card.h

FORMS += \
    mainwindow.ui \
    part10_favorite.ui \
    part5_cart.ui \
    part6_order_complete.ui \
    part7_store_page.ui \
    part8_menu_option_page.ui \
    part9_write_review.ui \
    w_grid_category.ui \
    w_h_category.ui \
    w_v_addr_card.ui \
    w_v_menu_card.ui \
    w_v_store_card.ui \
    my_page.ui \
    part2_address.ui \
    part3_search.ui \
    part4_clicked_cat_page.ui \
    part1_login_signup.ui \
    w_v_review_card.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    category.qrc \
    category_img.qrc \
    kyochon.qrc \
    mainIcon.qrc \
    review.qrc \
    store49.qrc

DISTFILES += \
    mainIcon/StartIMG.png \
    mainIcon/centered_security_icon.png \
    mainIcon/icon_0_0_transparent.png \
    mainIcon/icon_0_1_transparent.png \
    mainIcon/icon_0_2_transparent.png \
    mainIcon/icon_1_0_transparent.png \
    mainIcon/icon_1_1_transparent.png \
    mainIcon/icon_1_2_transparent.png \
    mainIcon/icon_2_0_transparent.png \
    mainIcon/icon_2_1_transparent.png \
    mainIcon/icon_2_2_transparent.png \
    mainIcon/notice.jpg \
    mainIcon/notice.png

