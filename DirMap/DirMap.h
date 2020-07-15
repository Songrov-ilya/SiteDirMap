#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDebug>

#include "DirWorker.h"

class DirMap : public QObject
{
    Q_OBJECT
    unsigned int maxAllWorkersDir;
    unsigned int haveAllWorkersDirFinished;
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

signals:
    void findDirs();
};

