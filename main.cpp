#include <QApplication>

#include "Creator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Creator creator;
    creator.start(a.arguments());

    return a.exec();
}
