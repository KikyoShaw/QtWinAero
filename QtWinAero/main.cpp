#include "QtWinAero.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWinAero w;
    w.show();
    return a.exec();
}
