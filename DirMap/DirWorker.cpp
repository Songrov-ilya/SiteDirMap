#include "DirWorker.h"

DirWorker::DirWorker(const unsigned int nameWorker, NodeDir *nodeDirRoot, QVector<NodeDir*> *vecUnexploredNodesDir,
                     QMutex *mutexUnexplored) :
    QObject(nullptr),
    name(nameWorker),
    nodeDirRootPtr(nodeDirRoot),
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
        return;
    }
    unexploredNode = vecUnexploredNodesDirPtr->last();
    vecUnexploredNodesDirPtr->pop_back();
    mutexDirUnexploredPtr->unlock();
    qDebug() << "walk" << name << Qt::endl;

    emit walkStarted(name);
    findChildren(unexploredNode);
}

void DirWorker::findChildren(NodeDir *exploredNode)
{
    qDebug() << "findChildren" << Qt::endl;
    QStringList listChildrenFolders = getFolders(exploredNode->myPath);
    if (listChildrenFolders.isEmpty()) {
        emit walkFinished(name);
        return;
    }
    if (listChildrenFolders.size() == 1) {
        NodeDir node(listChildrenFolders.last());
        exploredNode->vecChildren.append(node);
        findChildren(&exploredNode->vecChildren.last());
    }
    mutexDirUnexploredPtr->lock();
    for (int var = 0; var < listChildrenFolders.size() - 1; ++var) {
        NodeDir node(listChildrenFolders.at(var));
        exploredNode->vecChildren.append(node);
        vecUnexploredNodesDirPtr->append(&exploredNode->vecChildren.last());
    }
    mutexDirUnexploredPtr->unlock();

    NodeDir node(listChildrenFolders.last());
    exploredNode->vecChildren.append(node);
    findChildren(&exploredNode->vecChildren.last());
    emit unexploredNodeAppeared();
}

QStringList DirWorker::getFolders(const QString &path)
{
    qDebug() << "folders" << QDir(path).entryList(QDir::Dirs | QDir::NoDotAndDotDot) << Qt::endl;
    return QDir(path).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
}
