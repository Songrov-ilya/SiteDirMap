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
    QString result;
public:
    explicit DirMap(QObject *parent = nullptr);
    ~DirMap();

    void create(const QString &targetDir);

private slots:
    void workerFinished(const unsigned int nameWorker);
    void goIdleWorkerForWalk();
private:
    inline void createWorkersThreads();
    void showDirMap();
    void showChildren(const NodeDir *node, const int indent = 3, const QChar &charSpace = ' ');

signals:
    void findDirs();
    void resultIsReady(const QString &result);
};

