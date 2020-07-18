#pragma once

#include <QObject>
#include <QMutex>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

#include "NodeDir.h"

class DirWorker : public QObject
{
    Q_OBJECT

    const unsigned int name;
    QVector<NodeDir*> *vecUnexploredNodesDirPtr;
    QMutex *mutexDirUnexploredPtr;
public:
    explicit DirWorker(const unsigned int nameWorker, QVector<NodeDir*> *vecUnexploredNodesDir, QMutex *mutexDirUnexplored);

public slots:
    void walk();

private:
    void findChildren(NodeDir *exploredNode);
    inline QStringList getFolders(const QString &path);
signals:
    void walkFinished(const unsigned int nameWorker);
    void unexploredNodeAppeared();
};

