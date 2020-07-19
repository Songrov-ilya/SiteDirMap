#include "DirMap.h"

DirMap::DirMap(QObject *parent) : QObject(parent)
{
    createWorkersThreads();
}

DirMap::~DirMap()
{
    for (QThread *threadWorker : vecThreads) {
        threadWorker->quit();
        threadWorker->wait();
        delete threadWorker;
    }
}

void DirMap::create(const QString &targetDir)
{
    if(!QDir(targetDir).exists()){
        qDebug() << "sory, your target dir is not exist" << Qt::endl;
        return;
    }
    nodeDirRoot.setMyPath(targetDir);
    vecUnexploredNodesDir.append(&nodeDirRoot);
    goIdleWorkerForWalk();
}

void DirMap::goIdleWorkerForWalk()
{
    QVector<int> vecIdleWorkers;
    for (int var = 0; var < bitHaveAllWorkersFinished.size(); ++var) {
        if (!bitHaveAllWorkersFinished.testBit(var)) {
            vecIdleWorkers.append(var);
        }
    }
    QObject::disconnect(this, &DirMap::findDirs, nullptr, nullptr);
    mutexDirUnexplored.lock();
    const int min = std::min(vecUnexploredNodesDir.size(), vecIdleWorkers.size());
    mutexDirUnexplored.unlock();
    for (int var = 0; var < min; ++var) {
        connect(this, &DirMap::findDirs, vecDirWorkers.at(vecIdleWorkers.at(var)), &DirWorker::walk);
        bitHaveAllWorkersFinished.setBit(vecIdleWorkers.at(var));
    }
    emit findDirs();
}

void DirMap::workerFinished(const unsigned nameWorker)
{
    bitHaveAllWorkersFinished.clearBit(nameWorker);
    for (int var = 0; var < bitHaveAllWorkersFinished.size(); ++var) {
        if(bitHaveAllWorkersFinished.testBit(var)){
            return;
        }
    }
    showDirMap();
    qDebug() << Qt::endl << Qt::endl << "finish:)" << Qt::endl;
}

void DirMap::createWorkersThreads()
{
    unsigned int processor_count = std::thread::hardware_concurrency();
    if (processor_count > 1) {
        --processor_count; // first thread == maing thread, 2...n == workers
    }
    qDebug() << QString("Please wait... (processor_count %1)").arg(processor_count) << Qt::endl;
    bitHaveAllWorkersFinished.resize(processor_count);
    vecDirWorkers.reserve(processor_count);
    for (unsigned int var = 0; var < processor_count; ++var) {
        QThread *threadWorker = new QThread();
        DirWorker *worker = new DirWorker(var, &vecUnexploredNodesDir, &mutexDirUnexplored);
        worker->moveToThread(threadWorker);
        connect(threadWorker, &QThread::finished, worker, &QObject::deleteLater);
        connect(worker, &DirWorker::walkFinished, this, &DirMap::workerFinished);
        connect(worker, &DirWorker::unexploredNodeAppeared, this, &DirMap::goIdleWorkerForWalk);
        threadWorker->start();
        vecThreads.append(threadWorker);
        vecDirWorkers.append(worker);
    }
}

void DirMap::showDirMap()
{
    qDebug() << 1 << nodeDirRoot.getBasenameMyPath(Node::DIR);
    result.append(nodeDirRoot.getBasenameMyPath(Node::DIR) + '\n');
    showChildren(&nodeDirRoot, 3, '-');
    resultIsReady(result);
}

void DirMap::showChildren(const Node *node, const int indent, const QChar &charSpace)
{
    static const int indTree { indent };
    static int number { 1 };
    for (const Node *n: node->getVecChildren()) {
        const QString line { QString("%1%2").arg(QString(indent, charSpace)).arg(n->getBasenameMyPath(Node::DIR)) };
        result.append(line + '\n');
        qDebug() << ++number << line;
        showChildren(n, indent + indTree, charSpace);
    }
}

