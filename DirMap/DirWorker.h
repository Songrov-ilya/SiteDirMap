#pragma once

#include <QObject>
#include <QMutex>
#include <QDir>
#include <QDebug>

#include "Node.h"

class DirWorker : public QObject
{
    Q_OBJECT

    const unsigned int name;
    QVector<Node*> *vecUnexploredNodesDirPtr;
    QMutex *mutexDirUnexploredPtr;
public:
    explicit DirWorker(const unsigned int nameWorker, QVector<Node*> *vecUnexploredNodesDir, QMutex *mutexDirUnexplored);

public slots:
    void walk();

private:
    void findChildren(Node *exploredNode);
    inline QStringList getFolders(const QString &path);
signals:
    void walkFinished(const unsigned int nameWorker);
    void unexploredNodeAppeared();
};

