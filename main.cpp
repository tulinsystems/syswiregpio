#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QApplication>
#include <QDir>
#include <iostream>
#include "layout_widg.h"
#include "center.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("plastique");

    layout_widg window;

    window.setWindowTitle("GPIO Testing Utility");
    window.show();

    center(window);

    return a.exec();
}
