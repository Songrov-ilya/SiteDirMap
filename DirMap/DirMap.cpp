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

}

void DirMap::writeResults()
{
    for (const QThread *th: vecThreads) {
        if (!th->isRunning()) {
            mutexDir.lock();
            if (!vecUnexploredNodes.isEmpty()) {
                findDirs(vecUnexploredNodes.first());
            }
            mutexDir.unlock();
        }
    }
//    vecUnexploredNodes
}

void DirMap::createWorkersThreads()
{
    const auto processor_count = std::thread::hardware_concurrency();
    for (unsigned int var = 0; var < processor_count; ++var) {
        QThread *threadWorker = new QThread();
        DirWorker *worker = new DirWorker();
        connect(threadWorker, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &DirMap::findDirs, worker, &DirWorker::walk);
        connect(worker, &DirWorker::resultReady, this, &DirMap::writeResults);
        threadWorker->start();
        vecThreads.append(threadWorker);
    }
}
