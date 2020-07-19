#include "Node.h"

Node::Node() {}

Node::Node(const QString &path) : myPath(path)
{
}

Node::~Node()
{
    for (Node *node: vecChildren) {
        delete node;
    }
}

void Node::setMyPath(const QString &path)
{
    this->myPath = path;
}

void Node::addChild(Node *node)
{
    vecChildren.append(node);
}

const QVector<Node *> Node::getVecChildren() const
{
    return vecChildren;
}

const QString &Node::getMyPath() const
{
    return this->myPath;
}

QString Node::getBasenameMyPath(const Node::TypeNode typeNode) const
{
    QString ret;
    if (typeNode == TypeNode::DIR) {
        ret = QFileInfo(myPath).baseName();
    }
    else{
        ret = QUrl(myPath).path();
    }
    return ret;
}

Node *Node::getLastChild() const
{
    return vecChildren.last();
}
