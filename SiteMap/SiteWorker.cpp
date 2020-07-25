#include "SiteWorker.h"

SiteWorker::SiteWorker(const unsigned int nameWorker, const QString &rootUrl, QSet<QString> *setAllUrls,
                       QVector<Node *> *vecUnexploredNodesSite, QMutex *mutexSiteUnexplored) :
    QObject(nullptr),
    nameIndex(nameWorker),
    rootUrl(rootUrl),
    setAllUrls(setAllUrls),
    vecUnexploredNodesSitePtr(vecUnexploredNodesSite),
    mutexSiteUnexploredPtr(mutexSiteUnexplored),
    currentExploredNode(nullptr),
    useDuplicateUrls(false)
{

}

void SiteWorker::walk()
{
    Node *unexploredNode;
    mutexSiteUnexploredPtr->lock();
    if (vecUnexploredNodesSitePtr->isEmpty()) {
        mutexSiteUnexploredPtr->unlock();
        emit walkFinished(nameIndex);
        return;
    }
    unexploredNode = vecUnexploredNodesSitePtr->last();
    vecUnexploredNodesSitePtr->pop_back();
    mutexSiteUnexploredPtr->unlock();

    findChildren(unexploredNode);
}

void SiteWorker::findChildren(Node *exploredNode)
{
    static QNetworkAccessManager mngr;
    static bool notConnected { true };
    if (notConnected) {
        connect(&mngr, &QNetworkAccessManager::sslErrors, this, &SiteWorker::onIgnoreSSLErrors);
        connect(&mngr, &QNetworkAccessManager::finished, this, &SiteWorker::getResponse);
        notConnected = false;
    }

    currentExploredNode = exploredNode;
    qDebug() << Qt::endl << "*** find ***" << exploredNode->getMyPath() << Qt::endl;
    QNetworkRequest request(exploredNode->getMyPath());
    mngr.get(request);
}

void SiteWorker::parseResponse(QStringList *listInternalLinks, QStringList *listExternalLinks, const QByteArray &arr)
{
    QTextDocument doc;
    doc.setHtml(arr);
    searchLink(listInternalLinks, listExternalLinks, doc.rootFrame());
}

void SiteWorker::getResponse(QNetworkReply *reply)
{
    QByteArray byteArrReply(reply->readAll());
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError){
        qDebug() << "reply->error()" << reply->error() << Qt::endl;
        return;
    }
    //    qDebug() << "byteArrReply" << qPrintable(byteArrReply) << Qt::endl;
    QStringList listInternalLinks;
    QStringList listExternalLinks;
    parseResponse(&listInternalLinks, &listExternalLinks, byteArrReply);
    handlingChildren(listInternalLinks, listExternalLinks);
}

void SiteWorker::handlingChildren(const QStringList listInternalLinks, const QStringList listExternalLinks)
{
    for (const QString &external : listExternalLinks) {
        Node *node = new Node(external);
        currentExploredNode->addChild(node);
    }
    if (listInternalLinks.size() > 1) {
        mutexSiteUnexploredPtr->lock();
        for (int var = 0; var < listInternalLinks.size() - 1; ++var) {
            Node *node = new Node(listInternalLinks.at(var));
            currentExploredNode->addChild(node);
            vecUnexploredNodesSitePtr->append(currentExploredNode->getLastChild());
        }
        mutexSiteUnexploredPtr->unlock();
        emit unexploredNodeAppeared();
    }
    if (listInternalLinks.isEmpty()) {
        walk();
        return;
    }

    Node *node = new Node(listInternalLinks.last());
    currentExploredNode->addChild(node);
    findChildren(currentExploredNode->getLastChild());
}

void SiteWorker::onIgnoreSSLErrors(QNetworkReply *reply, QList<QSslError> error)
{
    qDebug() << "SSLErrors:" << error << reply << Qt::endl;
    reply->ignoreSslErrors(error);
}


void SiteWorker::searchLink(QStringList *listInternalLinks, QStringList *listExternalLinks, const QTextFrame *parent)
{
    for( QTextFrame::iterator it = parent->begin(); !it.atEnd(); ++it ){
        QTextFrame *textFrame = it.currentFrame();
        QTextBlock textBlock = it.currentBlock();

        if( textFrame ){
            searchLink(listInternalLinks, listExternalLinks, textFrame);
        }
        else if( textBlock.isValid() ){
            searchLink(listInternalLinks, listExternalLinks, textBlock);
        }
    }
}

void SiteWorker::searchLink(QStringList *listInternalLinks, QStringList *listExternalLinks, const QTextBlock &parent)
{
    for(QTextBlock::iterator it = parent.begin(); !it.atEnd(); ++it){
        QTextFragment textFragment = it.fragment();
        if( textFragment.isValid() ){
            QTextCharFormat textCharFormat = textFragment.charFormat();
            if( textCharFormat.isAnchor() ){
                const QString href = textCharFormat.anchorHref();
                QUrl url = QUrl::fromUserInput(href);
                if(url.isValid() && !url.isLocalFile()){
                    const QString urlStr = url.toString();
                    const int oldCount = setAllUrls->count();
                    setAllUrls->insert(urlStr);
                    const bool isNewUrl = setAllUrls->count() > oldCount;
                    if (isNewUrl) { /* finish at the end */ // mutex
                        if (urlStr.leftRef(rootUrl.size()).contains(rootUrl)) { /* finish at the end */ // https:// != http://
                            static int numUrl { 0 };
                            qDebug() << ++numUrl << urlStr;
                            listInternalLinks->append(urlStr);
                            continue;
                        }
                    }
                    if (isNewUrl || useDuplicateUrls) {
                        listExternalLinks->append(urlStr);
                    }
                }
            }
        }
    }
}
