#include "mainwindow.h"
// #include "rider_make_id.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // rider_make_id w;
    MainWindow w;
    w.show();
    return a.exec();
}
