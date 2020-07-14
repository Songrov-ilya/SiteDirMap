#pragma once

#include <QObject>

#include "NodeDir.h"

class DirWorker : public QObject
{
    Q_OBJECT
public:
    explicit DirWorker(QObject *parent = nullptr);

public slots:
    void walk(NodeDir *nodeDir);

signals:
    void resultReady();
};

