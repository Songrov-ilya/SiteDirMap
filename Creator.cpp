#include "Creator.h"

Creator::Creator(QObject *parent) :
    QObject(parent),
    dirMap (nullptr),
    siteMap(nullptr)
{

    qDebug() << "start" << Qt::endl;
}

Creator::~Creator()
{
    delete dirMap;
    delete siteMap;
}

void Creator::start(const QStringList &arguments)
{
#ifdef QT_DEBUG
    QStringList &arg = const_cast<QStringList&>(arguments);
    arg << "--dir" << "-d" << "/media/songrov/1478E91378E8F500/IlyaFolder/Songrov_Ilya/Programming/QtProjects/SiteDirMap" << "-f" <<
        "/media/songrov/1478E91378E8F500/IlyaFolder/Songrov_Ilya/Programming/QtProjects/SiteDirMap/fileOutput.txt";
#endif

    QString flag;
    if (arguments.size() > 1) {
        flag = arguments.at(1);
    }

    if (flag == "--dir") {
        const int indexDir = arguments.indexOf("-d");
        const int indexFile = arguments.indexOf("-f");
        if (indexDir == -1 || indexFile == -1 || indexDir + 1 >= arguments.size() || indexDir + 1 >= arguments.size()) {
            qDebug() << "Not valide arguments" << Qt::endl;
            return;
        }
        dirMap = new DirMap();
        dirMap->create(arguments.at(indexDir + 1), arguments.at(indexFile + 1));
    }
}
