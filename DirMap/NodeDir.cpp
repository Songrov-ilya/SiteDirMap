#include "NodeDir.h"

NodeDir::NodeDir() {}

NodeDir::NodeDir(const QString &path) : myPath(path)
{
}

NodeDir::~NodeDir()
{
    for (NodeDir *node: vecChildren) {
        delete node;
    }
}

void NodeDir::setMyPath(const QString &path)
{
    this->myPath = path;
}

void NodeDir::addChild(NodeDir *nodeDir)
{
    vecChildren.append(nodeDir);
}

const QVector<NodeDir *> NodeDir::getVecChildren() const
{
    return vecChildren;
}

const QString &NodeDir::getMyPath() const
{
    return this->myPath;
}

QString NodeDir::getBasenameMyPath() const
{
    return QFileInfo(myPath).baseName();
}

NodeDir *NodeDir::getLastChild() const
{
    return vecChildren.last();
}
