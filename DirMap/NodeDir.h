#pragma once

#include <QString>
#include <QVector>

struct NodeDir{
    QString pathDir;
    QVector<NodeDir> vecChildren;
};

