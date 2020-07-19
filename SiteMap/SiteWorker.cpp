#include "SiteWorker.h"

SiteWorker::SiteWorker(const unsigned int nameWorker, QVector<Node *> *vecUnexploredNodesSite, QMutex *mutexSiteUnexplored) :
    QObject(nullptr),
    name(nameWorker),
    vecUnexploredNodesSitePtr(vecUnexploredNodesSite),
    mutexSiteUnexploredPtr(mutexSiteUnexplored),
    currentExploredNode(nullptr)
{

}

void SiteWorker::walk()
{
    Node *unexploredNode;
    mutexSiteUnexploredPtr->lock();
    if (vecUnexploredNodesSitePtr->isEmpty()) {
        mutexSiteUnexploredPtr->unlock();
        emit walkFinished(name);
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
    QNetworkRequest request(exploredNode->getMyPath());
    mngr.get(request);
}

QStringList SiteWorker::parseResponse(const QByteArray &arr)
{
    /*
     * Реклама</a><a href="http://www.google.com.ua/intl/uk/services/">Рішення для бізнесу</a><a href="/intl/uk/about.html">
     * Усе про Google</a><a href="https://www.google.com/setprefdomain?prefdom=UA&amp;
     * prev=https://www.google.com.ua/&amp;sig=K_LlgHsLYXq3mcJP9L6UPVeRpLL08%3D">Google.com.ua</a>
     * </div></div><p style="font-size:8pt;color:#767676">&copy; 2020</p></span></center><script nonce="v3+XNVzckgioOLx1HycYeQ==">
     * */
//#ifdef QT_DEBUG
//    QFile file("../fileSite.txt");
//    file.open(QFile::WriteOnly);
//    file.write(arr);
//    file.close();
//#endif

//    QWebEngineView *view = new QWebEngineView();
//        view->load(QUrl("http://qt-project.org/"));
//        view->shogw();

    QStringList list;
//    QRegularExpression rx("(www.google.com.ua)");
//    QRegularExpression rx("/(http|https|ftp|ftps)\:\/\/[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(\/\S*)?/");
//    QRegularExpression rx("(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?");
//    QRegularExpression rx("(?:(?:https?|ftp):\/\/)?[\w/\-?=%.]+\.[\w/\-?=%.]+");
//    QRegularExpression rx("/(http|https|ftp|ftps)\\:\\/\\/[a-zA-Z0-9\\-\\.]+\\.[a-zA-Z]{2,3}(\\/\\S*)?/");

    QRegularExpression rx("((?:https?|ftp)://\\S+)");
    QRegularExpressionMatchIterator rxIterator = rx.globalMatch(arr);
    while (rxIterator.hasNext()) {
        QRegularExpressionMatch match = rxIterator.next();
        QString link = match.captured(1);
        list << link;
    }
//    list << "https://www.google.com/";
    return list;
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
    handlingChildren(parseResponse(byteArrReply));
}

void SiteWorker::handlingChildren(const QStringList listChildrenLinks)
{
    qDebug() << "listChildrenLinks" << qPrintable(listChildrenLinks.join("\n")) << Qt::endl;
    if (listChildrenLinks.isEmpty()) {
        walk();
        return;
    }
    if (listChildrenLinks.size() > 1) {
        mutexSiteUnexploredPtr->lock();
        for (int var = 0; var < listChildrenLinks.size() - 1; ++var) {
            Node *node = new Node(currentExploredNode->getMyPath() + "/" + listChildrenLinks.at(var));
            currentExploredNode->addChild(node);
            vecUnexploredNodesSitePtr->append(currentExploredNode->getLastChild());
        }
        mutexSiteUnexploredPtr->unlock();
        emit unexploredNodeAppeared();
    }

    Node *node = new Node(listChildrenLinks.last());
    currentExploredNode->addChild(node);
    findChildren(currentExploredNode->getLastChild());
}

void SiteWorker::onIgnoreSSLErrors(QNetworkReply *reply, QList<QSslError> error)
{
    qDebug() << "SSLErrors:" << error << reply << Qt::endl;
    reply->ignoreSslErrors(error);
}
