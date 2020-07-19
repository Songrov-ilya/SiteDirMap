#pragma once

#include <QString>
#include <QList>
#include <QFileInfo>

struct NodeDir{
private:
    QVector<NodeDir*> vecChildren;
    QString myPath;
public:
    NodeDir();
    NodeDir(const QString &path);
    ~NodeDir();

    void setMyPath(const QString &path);
    void addChild(NodeDir *nodeDir);
    const QVector<NodeDir*> getVecChildren() const;
    const QString &getMyPath() const;
    QString getBasenameMyPath() const;
    NodeDir *getLastChild() const;
};

