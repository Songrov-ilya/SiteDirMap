#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDebug>

#include "DirWorker.h"

class DirMap : public QObject
{
    Q_OBJECT
    QVector<QThread*> vecThreads;
    NodeDir nodeDir;
    QVector<NodeDir*> vecUnexploredNodes;
    QMutex mutexDir;
public:
    explicit DirMap(QObject *parent = nullptr);
    ~DirMap();

    void create(const QString &targetDir, const QString &fileOutput);

private slots:
    void writeResults();
private:
    inline void createWorkersThreads();

signals:
    void findDirs(NodeDir *nodeDir, QVector<NodeDir*> vecUnexploredNodes, QMutex *mutexDir);
};

