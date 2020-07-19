#include "Creator.h"

Creator::Creator(QObject *parent) :
    QObject(parent),
    dirMap (nullptr),
    siteMap(nullptr)
{

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
    arg << "--dir" << "-t" << "/home" << "-f" << "../fileOutputDir.txt";
#endif

    QString flag;
    if (arguments.size() > 1) {
        flag = arguments.at(1);
    }

    if (flag == "--dir") {
        const int indexDir = arguments.indexOf("-t");
        const int indexFile = arguments.indexOf("-f");
        if (indexDir == -1 || indexFile == -1 || indexDir + 1 >= arguments.size() || indexDir + 1 >= arguments.size()) {
            qDebug() << "Not valide arguments" << Qt::endl;
            exit(0);
        }
        dirMap = new DirMap();
        connect(dirMap, &DirMap::resultIsReady, this, &Creator::writeResult);
        fileOutput = arguments.at(indexFile + 1);
        dirMap->create(arguments.at(indexDir + 1));
    }
    else if (flag == "-h") {
        qDebug() << "--dir      : creates a dirmap" << Qt::endl;
        qDebug() << "--site     : creates a sitemap" << Qt::endl;
        qDebug() << "-t         : target dir or site (</home> or <https://www.google.com/>)" << Qt::endl;
        qDebug() << "-f         : file output" << Qt::endl;
        qDebug() << "-h         : help" << Qt::endl;
    }
}

void Creator::writeResult(const QString &result)
{
    if (fileOutput.isEmpty()) {
        return;
    }
    QFile file(fileOutput);
    if (!file.open(QFile::WriteOnly)) {
        qWarning() << "Error, file is not open" << file.fileName() << Qt::endl;
        return;
    }
    file.write(result.toUtf8());
    file.close();
}
