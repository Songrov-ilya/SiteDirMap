#pragma once

#include <QObject>
#include <QDebug>

#include "SiteMap.h"
#include "DirMap.h"

class Creator : public QObject
{
    Q_OBJECT

    DirMap  *dirMap ;
    SiteMap *siteMap;
public:
    explicit Creator(QObject *parent = nullptr);
    ~Creator();

    void start(const QStringList &arguments);
};

