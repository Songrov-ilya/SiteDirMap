#pragma once

#include <QString>
#include <QList>
#include <QFileInfo>

struct Stack{

};

struct NodeDir{
private:
    QVector<NodeDir*> vecChildren;
    QString myPath;
    QString myPathBaseName;
public:
    NodeDir();
    NodeDir(const QString &path);
    ~NodeDir();

    void setMyPath(const QString &path);
    void addChild(NodeDir *nodeDir);
    const QVector<NodeDir*> getVecChildren() const; /* finish at the end */ // remove this
    const QString &getMyPath() const;
    const QString &getBasenameMyPath() const;
    NodeDir *getLastChild() const;
};

