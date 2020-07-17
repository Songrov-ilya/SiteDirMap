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
    QBitArray bitHaveAllWorkersFinished;
    QVector<QThread*> vecThreads;
    NodeDir nodeDirRoot;
    QVector<NodeDir*> vecUnexploredNodesDir;
    QMutex mutexDirUnexplored;
public:
    explicit DirMap(QObject *parent = nullptr);
    ~DirMap();

    void create(const QString &targetDir, const QString &fileOutput);

private slots:
    void workerStarted(const unsigned int nameWorker);
    void workerFinished(const unsigned int nameWorker);
private:
    inline void createWorkersThreads();
    inline void createThreadsConnects();
    void showDirMap();
    void showChildren(const NodeDir *node, const int indent);

signals:
    void findDirs();
};

