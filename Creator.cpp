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
//    arg << "--dir" << "-t" << "/home" << "-f" << "../fileOutputDir.txt";
//    arg << "--site" << "-t" << "https://www.google.com" << "-f" << "../fileOutputSite.txt";
//    arg << "--site" << "-t" << "https://allbible.info/bible/sinodal" << "-f" << "../fileOutputSite.txt";
//    arg << "--site" << "-t" << "https://////allbible.info/bible/sinodal/jos/13/" << "-f" << "../fileOutputSite.txt";
    arg << "--site" << "-t" << "https://github.com/Ilya-Songrov" << "-f" << "../fileOutputSite.txt";
//    arg << "--site" << "-t" << "https://allbible.info/bible/kingjames/mt/20/#!prettyPhoto[iframes]/2/" << "-f" << "../fileOutputSite.txt";
#endif

    QString flag;
    if (arguments.size() > 1) {
        flag = arguments.at(1);
    }

    if (arguments.contains("--dir") || arguments.contains("--site")) {
        const int indexTarget = arguments.indexOf("-t");
        const int indexFile = arguments.indexOf("-f");
        if (indexTarget == -1 || indexFile == -1 || indexTarget + 1 >= arguments.size() || indexTarget + 1 >= arguments.size()) {
            qDebug() << "Not valide arguments" << Qt::endl;
            exit(0);
        }
        fileOutput = arguments.at(indexFile + 1);
        const QString target { arguments.at(indexTarget + 1) };
        if (arguments.contains("--dir")) {
            dirMap = new DirMap();
            connect(dirMap, &DirMap::resultIsReady, this, &Creator::writeResult);
            dirMap->create(target);
            return;
        }
        siteMap = new SiteMap();
        connect(siteMap, &SiteMap::resultIsReady, this, &Creator::writeResult);
        siteMap->create(target, arguments.contains("--duplicate"));
    }
    else if (flag == "-h") {
        qDebug() << "--dir          : creates a dirmap" << Qt::endl;
        qDebug() << "--site         : creates a sitemap" << Qt::endl;
        qDebug() << "-t             : target dir or site (</home> or <https://www.google.com/>)" << Qt::endl;
        qDebug() << "-f             : file output" << Qt::endl;
        qDebug() << "--duplicate    : if you use this flag then duplicates (site/dir) will be recorded." << Qt::endl;
        qDebug() << "-h             : help" << Qt::endl;
        exit(0);
    }
    else{
        qDebug() << "Not valide arguments" << Qt::endl;
        exit(0);
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
