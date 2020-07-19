#pragma once

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRegularExpression>

#ifdef QT_DEBUG
//#include <QWebEngineView>
#include <QFile>
#endif

#include "Node.h"

class SiteWorker : public QObject
{
    Q_OBJECT

    const unsigned int name;
    QVector<Node*> *vecUnexploredNodesSitePtr;
    QMutex *mutexSiteUnexploredPtr;
    Node *currentExploredNode;
public:
    explicit SiteWorker(const unsigned int nameWorker, QVector<Node*> *vecUnexploredNodesSite, QMutex *mutexSiteUnexplored);

public slots:
    void walk();

private:
    void findChildren(Node *currentExploredNode);
    QStringList parseResponse(const QByteArray &arr);
private slots:
    void getResponse(QNetworkReply *reply);
    void handlingChildren(const QStringList listChildrenLinks);
    void onIgnoreSSLErrors(QNetworkReply *reply, QList<QSslError> error);
signals:
    void walkFinished(const unsigned int nameWorker);
    void unexploredNodeAppeared();
};

