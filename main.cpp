#include <QApplication>

#include "Creator.h"

#define VERSION_MAJOR    "0"
#define VERSION_MINOR    "1"
#define VERSION_BUILD    "0"
#define APP_VERSION QString(VERSION_MAJOR) + "." + QString(VERSION_MINOR) + "." + QString(VERSION_BUILD)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(APP_VERSION);

    Creator creator;
    creator.start(a.arguments());

    return a.exec();
}
