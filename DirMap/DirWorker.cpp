#include "DirWorker.h"

DirWorker::DirWorker(const unsigned int nameWorker, QVector<Node*> *vecUnexploredNodesDir, QMutex *mutexUnexplored) :
    QObject(nullptr),
    name(nameWorker),
    vecUnexploredNodesDirPtr(vecUnexploredNodesDir),
    mutexDirUnexploredPtr(mutexUnexplored)
{

}

void DirWorker::walk()
{
    Node *unexploredNode;
    mutexDirUnexploredPtr->lock();
    if (vecUnexploredNodesDirPtr->isEmpty()) {
        mutexDirUnexploredPtr->unlock();
        emit walkFinished(name);
        return;
    }
    unexploredNode = vecUnexploredNodesDirPtr->last();
    vecUnexploredNodesDirPtr->pop_back();
    mutexDirUnexploredPtr->unlock();

    findChildren(unexploredNode);
}

void DirWorker::findChildren(Node *exploredNode)
{
    QStringList listChildrenFolders = getFolders(exploredNode->getMyPath());
    if (listChildrenFolders.isEmpty()) {
        walk();
        return;
    }
    if (listChildrenFolders.size() > 1) {
        mutexDirUnexploredPtr->lock();
        for (int var = 0; var < listChildrenFolders.size() - 1; ++var) {
            Node *node = new Node(exploredNode->getMyPath() + "/" + listChildrenFolders.at(var));
            exploredNode->addChild(node);
            vecUnexploredNodesDirPtr->append(exploredNode->getLastChild());
        }
        mutexDirUnexploredPtr->unlock();
        emit unexploredNodeAppeared();
    }

    Node *node = new Node(exploredNode->getMyPath() + "/" + listChildrenFolders.last());
    exploredNode->addChild(node);
    findChildren(exploredNode->getLastChild());
}

QStringList DirWorker::getFolders(const QString &path)
{
    if (path.isEmpty()) {
        return QStringList();
    }
    return QDir(path).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
}
