#include "SiteMap.h"

SiteMap::SiteMap(QObject *parent) : QObject(parent)
{

}

SiteMap::~SiteMap()
{
    for (QThread *threadWorker : vecThreads) {
        threadWorker->quit();
        threadWorker->wait();
        delete threadWorker;
    }
}

void SiteMap::create(const QString &targetSite)
{
    const QUrl url = QUrl::fromUserInput(targetSite);
    if(!url.isValid() && !url.isLocalFile()){
        qDebug() << "sory, your target site is not valid" << Qt::endl;
        return;
    }
    // (exemple) targetSite == https://www.google.com.ua/preferences?hl=ru --> mergeUrl == https://www.google.com.ua/preferences
    const QString mergeUrl = url.scheme() + "://" + url.host() + url.path();
    createWorkersThreads(mergeUrl);
    nodeSiteRoot.setMyPath(url.toString());
    vecUnexploredNodesSite.append(&nodeSiteRoot);
    goIdleWorkerForWalk();
}

void SiteMap::goIdleWorkerForWalk()
{
    QVector<int> vecIdleWorkers;
    for (int var = 0; var < bitHaveAllWorkersFinished.size(); ++var) {
        if (!bitHaveAllWorkersFinished.testBit(var)) {
            vecIdleWorkers.append(var);
        }
    }
    QObject::disconnect(this, &SiteMap::findLinks, nullptr, nullptr);
    mutexSiteUnexplored.lock();
    const int min = std::min(vecUnexploredNodesSite.size(), vecIdleWorkers.size());
    mutexSiteUnexplored.unlock();
    for (int var = 0; var < min; ++var) {
        connect(this, &SiteMap::findLinks, vecSiteWorkers.at(vecIdleWorkers.at(var)), &SiteWorker::walk);
        bitHaveAllWorkersFinished.setBit(vecIdleWorkers.at(var));
    }
    emit findLinks();
}

void SiteMap::workerFinished(const unsigned nameWorker)
{
    bitHaveAllWorkersFinished.clearBit(nameWorker);
    for (int var = 0; var < bitHaveAllWorkersFinished.size(); ++var) {
        if(bitHaveAllWorkersFinished.testBit(var)){
            return;
        }
    }
    showSiteMap();
    qDebug() << Qt::endl << Qt::endl << "finish:)" << Qt::endl;
}

void SiteMap::createWorkersThreads(const QString &rootUrl)
{
    unsigned int processor_count = std::thread::hardware_concurrency();
    if (processor_count > 1) {
        --processor_count; // first thread == maing thread, 2...n == workers
    }
    qDebug() << QString("Please wait... (processor_count %1)").arg(processor_count) << Qt::endl;
    bitHaveAllWorkersFinished.resize(processor_count);
    vecSiteWorkers.reserve(processor_count);
    for (unsigned int var = 0; var < processor_count; ++var) {
        QThread *threadWorker = new QThread();
        SiteWorker *worker = new SiteWorker(var, rootUrl, &setAllUrls, &vecUnexploredNodesSite, &mutexSiteUnexplored);
        worker->moveToThread(threadWorker);
        connect(threadWorker, &QThread::finished, worker, &QObject::deleteLater);
        connect(worker, &SiteWorker::walkFinished, this, &SiteMap::workerFinished);
        connect(worker, &SiteWorker::unexploredNodeAppeared, this, &SiteMap::goIdleWorkerForWalk);
        threadWorker->start();
        vecThreads.append(threadWorker);
        vecSiteWorkers.append(worker);
    }
}

void SiteMap::showSiteMap()
{
    qDebug() << 1 << nodeSiteRoot.getBasenameMyPath(Node::LINK);
    result.append(nodeSiteRoot.getBasenameMyPath(Node::LINK) + '\n');
    showChildren(&nodeSiteRoot, 3, '-');
    resultIsReady(result);
}

void SiteMap::showChildren(const Node *node, const int indent, const QChar &charSpace)
{
    static const int indTree { indent };
    static int number { 1 };
    for (const Node *n: node->getVecChildren()) {
        const QString line { QString("%1%2").arg(QString(indent, charSpace)).arg(n->getBasenameMyPath(Node::LINK)) };
        result.append(line + '\n');
        qDebug() << ++number << line;
        showChildren(n, indent + indTree, charSpace);
    }
}

