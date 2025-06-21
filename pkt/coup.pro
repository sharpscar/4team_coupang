QT       += core gui
QT      += core network
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    card.cpp \
    dashboard.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp\
    menumanage.cpp \
    ordercheck.cpp \
    orderresdialog.cpp \
    sign_up.cpp \
    tcpclient.cpp \
    timemanage.cpp

HEADERS += \
    card.h \
    dashboard.h \
    login.h \
    mainwindow.h\
    menumanage.h \
    ordercheck.h \
    orderresdialog.h \
    sign_up.h \
    tcpclient.h \
    timemanage.h
FORMS += \
    card.ui \
    dashboard.ui \
    login.ui \
    mainwindow.ui \
    menumanage.ui \
    ordercheck.ui \
    orderresdialog.ui \
    sign_up.ui \
    timemanage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    LOGO.qrc
