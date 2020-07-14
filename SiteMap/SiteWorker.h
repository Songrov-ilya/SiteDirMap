#pragma once

#include <QObject>

class SiteWorker : public QObject
{
    Q_OBJECT
public:
    explicit SiteWorker(QObject *parent = nullptr);

signals:

};

