#pragma once

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QUrl>
#include <QBitArray>

#include "SiteWorker.h"

class SiteMap : public QObject
{
    Q_OBJECT
    QVector<QThread*> vecThreads;
    QVector<SiteWorker*> vecSiteWorkers;
    QBitArray bitHaveAllWorkersFinished; // 1 == Started, 0 == Finished
    Node nodeSiteRoot;
    QVector<Node*> vecUnexploredNodesSite;
    QMutex mutexSiteUnexplored;
    QString result;
public:
    explicit SiteMap(QObject *parent = nullptr);
    ~SiteMap();

    void create(const QString &targetSite);

private slots:
    void workerFinished(const unsigned int nameWorker);
    void goIdleWorkerForWalk();
private:
    inline void createWorkersThreads();
    void showSiteMap();
    void showChildren(const Node *node, const int indent = 3, const QChar &charSpace = ' ');

signals:
    void findLinks();
    void resultIsReady(const QString &result);
};

