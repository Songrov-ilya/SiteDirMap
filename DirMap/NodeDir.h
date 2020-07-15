#pragma once

#include <QString>
#include <QVector>

struct NodeDir{
    QVector<NodeDir> vecChildren;
    QString myPath;
    NodeDir() {}
    NodeDir(const QString &path) : myPath(path) {}
};

