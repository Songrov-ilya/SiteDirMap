#include <QCoreApplication>

#include "Creator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Creator creator;
    creator.start(a.arguments());

    return a.exec();
}
