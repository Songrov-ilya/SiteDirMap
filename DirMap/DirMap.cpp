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
    qDebug() << "create" << Qt::endl;
    if(!QDir(targetDir).exists()){
        qDebug() << "sory, your target dir is not exist" << Qt::endl;
        return;
    }
    nodeDirRoot.setMyPath(targetDir);
    vecUnexploredNodesDir.append(&nodeDirRoot);
    emit findDirs();
}

void DirMap::workerStarted(const unsigned nameWorker)
{
    bitHaveAllWorkersFinished.clearBit(nameWorker);
    qDebug() << nameWorker << "nameWorker statrted" << Qt::endl;
}

void DirMap::workerFinished(const unsigned nameWorker)
{
    bitHaveAllWorkersFinished.setBit(nameWorker);
    qDebug() << nameWorker << "nameWorker finished" << Qt::endl;
    for (int var = 0; var < bitHaveAllWorkersFinished.size(); ++var) {
        if(!bitHaveAllWorkersFinished.testBit(var)){
            return;
        }
    }
    disconnect(this, nullptr, nullptr, nullptr);
    qDebug() << "all workers finished!" << Qt::endl;
//    showDirMap();
}

void DirMap::createWorkersThreads()
{
    /* finish at the end */ // 1 + 2 + ... + (n – 1) + n = n × (n+1) / 2
    unsigned int processor_count = std::thread::hardware_concurrency();
    processor_count = 2;
    bitHaveAllWorkersFinished.resize(processor_count);
//    if (maxAllWorkersDir > USHRT_MAX - 1) {
//        processor_count = USHRT_MAX - 1;
//    }
    qDebug() << "processor_count" << processor_count << Qt::endl;
    QVector<DirWorker*> vecDirWorker(processor_count, nullptr);
    for (unsigned int var = 0; var < processor_count; ++var) {
        QThread *threadWorker = new QThread();
        DirWorker *worker = new DirWorker(var, &vecUnexploredNodesDir, &mutexDirUnexplored);
        worker->moveToThread(threadWorker);
        connect(threadWorker, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &DirMap::findDirs, worker, &DirWorker::walk);
        connect(worker, &DirWorker::walkStarted, this, &DirMap::workerStarted);
        connect(worker, &DirWorker::walkFinished, this, &DirMap::workerFinished);
        threadWorker->start();
        vecThreads.append(threadWorker);
        vecDirWorker[var] = worker;
    }

    // create connects
    for (int varCurrent = 0; varCurrent < vecDirWorker.size(); ++varCurrent) {
        for (int varOther = 0; varOther < vecDirWorker.size(); ++varOther) {
            if (varCurrent != varOther) {
                connect(vecDirWorker.at(varCurrent), &DirWorker::unexploredNodeAppeared, vecDirWorker.at(varOther), &DirWorker::walk);
            }
        }
    }
}

void DirMap::createThreadsConnects()
{

}

void DirMap::showDirMap()
{
    qDebug() << nodeDirRoot.getBasenameMyPath();
    showChildren(&nodeDirRoot, 1);
}

void DirMap::showChildren(const NodeDir *node, const int indent)
{
    for (const NodeDir *n: node->getVecChildren()) {
        qDebug() << QString("%1%2").arg(QString(indent, ' ')).arg(n->getBasenameMyPath());
        showChildren(n, indent + 2);
    }
}

