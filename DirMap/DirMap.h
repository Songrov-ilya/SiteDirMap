#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QBitArray>

#include "DirWorker.h"

class DirMap : public QObject
{
    Q_OBJECT
    QVector<QThread*> vecThreads;
    QVector<DirWorker*> vecDirWorkers;
    QBitArray bitHaveAllWorkersFinished; // 1 == Started, 0 == Finished
    NodeDir nodeDirRoot;
    QVector<NodeDir*> vecUnexploredNodesDir;
    QMutex mutexDirUnexplored;
public:
    explicit DirMap(QObject *parent = nullptr);
    ~DirMap();

    void create(const QString &targetDir, const QString &fileOutput);

private slots:
    void workerFinished(const unsigned int nameWorker);
    void goIdleWorkerForWalk();
private:
    inline void createWorkersThreads();
    void showDirMap();
    void showChildren(const NodeDir *node, const int indent);

signals:
    void findDirs();
};

