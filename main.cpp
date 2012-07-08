#include <QtGui/QApplication>
#include "parsercore.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    parsercore w;
    w.show();

    return a.exec();
}
