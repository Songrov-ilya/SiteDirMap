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

void DirMap::create(const QString &targetDir, const QString &fileOutput)
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
        qDebug() << vecIdleWorkers.at(var) << "nameWorker started" << Qt::endl;
    }
    emit findDirs();
}

void DirMap::workerFinished(const unsigned nameWorker)
{
    bitHaveAllWorkersFinished.clearBit(nameWorker);
    qDebug() << nameWorker << "nameWorker finished" << Qt::endl;
    for (int var = 0; var < bitHaveAllWorkersFinished.size(); ++var) {
        if(bitHaveAllWorkersFinished.testBit(var)){
            return;
        }
    }
    qDebug() << "all workers finished!" << Qt::endl;
    showDirMap();
    qDebug() << "finish:)" << Qt::endl;
}

void DirMap::createWorkersThreads()
{
    unsigned int processor_count = std::thread::hardware_concurrency();
    processor_count = 3;
    bitHaveAllWorkersFinished.resize(processor_count);
    qDebug() << "processor_count" << processor_count << Qt::endl;
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
    qDebug() << nodeDirRoot.getBasenameMyPath();
    showChildren(&nodeDirRoot, 1);
}

void DirMap::showChildren(const NodeDir *node, const int indent)
{
    static int number { 1 };
    for (const NodeDir *n: node->getVecChildren()) {
        ++number;
        qDebug() << QString("%1%2-%3").arg(QString(indent, ' ')).arg(n->getBasenameMyPath()).arg(number);
        showChildren(n, indent + 2);
    }
}

