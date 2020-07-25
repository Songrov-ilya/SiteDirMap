#pragma once

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextBlock>
#include <QSet>

#include "Node.h"

class SiteWorker : public QObject
{
    Q_OBJECT

    const unsigned int nameIndex;
    const QUrl rootUrl;
    const QString rootUrlStr;
    QSet<QString> *setAllUrls;
    QVector<Node*> *vecUnexploredNodesSitePtr;
    QMutex *mutexSiteUnexploredPtr;
    Node *currentExploredNode;
    const bool useDuplicateUrls; // extension
public:
    explicit SiteWorker(const unsigned int nameWorker, const QUrl &rootUrl, const bool useDuplicate, QSet<QString> *setAllUrls,
                        QVector<Node*> *vecUnexploredNodesSite, QMutex *mutexSiteUnexplored);
public slots:
    void walk();

private:
    void findChildren(Node *currentExploredNode);
    void parseResponse(QStringList *listInternalLinks, QStringList *listExternalLinks, const QByteArray &arr);
    void searchLink(QStringList *listInternalLinks, QStringList *listExternalLinks, const QTextFrame *parent);
    void searchLink(QStringList *listInternalLinks, QStringList *listExternalLinks, const QTextBlock &parent);
private slots:
    void getResponse(QNetworkReply *reply);
    void handlingChildren(const QStringList listInternalLinks, const QStringList listExternalLinks);
    void onIgnoreSSLErrors(QNetworkReply *reply, QList<QSslError> error);
signals:
    void walkFinished(const unsigned int nameWorker);
    void unexploredNodeAppeared();
};

