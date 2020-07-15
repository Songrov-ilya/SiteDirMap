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
    nodeDirRoot.myPath = targetDir;
    vecUnexploredNodesDir.append(&nodeDirRoot);
    emit findDirs();
}

void DirMap::workerStarted(const unsigned nameWorker)
{
    qDebug() << nameWorker << "nameWorker statrted" << Qt::endl;
    haveAllWorkersDirFinished -= nameWorker;
}

void DirMap::workerFinished(const unsigned nameWorker)
{
    haveAllWorkersDirFinished += nameWorker;
    qDebug() << nameWorker << "nameWorker finished" << Qt::endl;
    if (haveAllWorkersDirFinished == maxAllWorkersDir) {
        qDebug() << "all workers finished!" << Qt::endl;
    }
}

void DirMap::createWorkersThreads()
{
    /* finish at the end */ // 1 + 2 + ... + (n – 1) + n = n × (n+1) / 2
    auto processor_count = std::thread::hardware_concurrency();
    maxAllWorkersDir = processor_count * (processor_count + 1) / 2;
    haveAllWorkersDirFinished = maxAllWorkersDir;
//    if (maxAllWorkersDir > USHRT_MAX - 1) {
//        processor_count = USHRT_MAX - 1;
//    }
    qDebug() << "processor_count" << processor_count << Qt::endl;
    QVector<DirWorker*> vecDirWorker(processor_count, nullptr);
    for (unsigned int var = 0; var < processor_count; ++var) {
        QThread *threadWorker = new QThread();
        DirWorker *worker = new DirWorker(var + 1, &nodeDirRoot, &vecUnexploredNodesDir, &mutexDirUnexplored);
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

