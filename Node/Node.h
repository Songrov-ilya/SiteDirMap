#pragma once

#include <QString>
#include <QList>
#include <QFileInfo>
#include <QUrl>

struct Node{
private:
    QVector<Node*> vecChildren;
    QString myPath;
public:
    enum TypeNode{
        DIR,
        LINK,
    };
    Node();
    Node(const QString &path);
    ~Node();

    void setMyPath(const QString &path);
    void addChild(Node *node);
    const QVector<Node*> getVecChildren() const;
    const QString &getMyPath() const;
    QString getBasenameMyPath(const TypeNode typeNode) const;
    Node *getLastChild() const;
};

