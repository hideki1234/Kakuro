#include "kkreditmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KkrEditMain w;
    w.show();

    return a.exec();
}
