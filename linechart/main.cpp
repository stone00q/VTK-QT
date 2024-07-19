#include "widget.h"
//#include "testwidget2.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    //testWidget2 test;
    //test.show();
    return a.exec();
}
