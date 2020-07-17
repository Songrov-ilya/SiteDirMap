#include "DirWorker.h"

DirWorker::DirWorker(const unsigned int nameWorker, QVector<NodeDir*> *vecUnexploredNodesDir, QMutex *mutexUnexplored) :
    QObject(nullptr),
    name(nameWorker),
    iAmWorking(false),
    vecUnexploredNodesDirPtr(vecUnexploredNodesDir),
    mutexDirUnexploredPtr(mutexUnexplored)
{

}

void DirWorker::walk()
{
    NodeDir *unexploredNode;
    mutexDirUnexploredPtr->lock();
    if (vecUnexploredNodesDirPtr->isEmpty()) {
        mutexDirUnexploredPtr->unlock();
        if (iAmWorking) {
            emit walkFinished(name);
            iAmWorking = false;
        }
        return;
    }
    unexploredNode = vecUnexploredNodesDirPtr->last();
    vecUnexploredNodesDirPtr->pop_back();
    mutexDirUnexploredPtr->unlock();

    if (!iAmWorking) {
        emit walkStarted(name);
        iAmWorking = true;
    }
    findChildren(unexploredNode);
}

void DirWorker::findChildren(NodeDir *exploredNode)
{
    QStringList listChildrenFolders = getFolders(exploredNode->getMyPath());
    if (listChildrenFolders.isEmpty()) {
        walk();
        return;
    }
    if (listChildrenFolders.size() == 1) {
        NodeDir *node = new NodeDir(exploredNode->getMyPath() + "/" + listChildrenFolders.last());
        exploredNode->addChild(node);
        findChildren(exploredNode->getLastChild());
    }
    mutexDirUnexploredPtr->lock();
    for (int var = 0; var < listChildrenFolders.size() - 1; ++var) {
        NodeDir *node = new NodeDir(exploredNode->getMyPath() + "/" + listChildrenFolders.at(var));
        exploredNode->addChild(node);
        vecUnexploredNodesDirPtr->append(exploredNode->getLastChild());
    }
    mutexDirUnexploredPtr->unlock();

    NodeDir *node = new NodeDir(exploredNode->getMyPath() + "/" + listChildrenFolders.last());
    exploredNode->addChild(node);
    emit unexploredNodeAppeared();
    findChildren(exploredNode->getLastChild());
}

QStringList DirWorker::getFolders(const QString &path)
{
    if (path.isEmpty()) {
        return QStringList();
    }
    QDir dir(path);
    QStringList list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    qDebug() << "list" << list << Qt::endl;
//    std::for_each(list.begin(), list.end(), [&path](QString &child){ child.insert(0, path + "/"); });
    return list;
}
