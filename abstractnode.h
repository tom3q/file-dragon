#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QString>

class AbstractNode
{
public:
    AbstractNode();

    void setParent(AbstractNode *);
    AbstractNode *getParent() const;

    void setName(const QString &);
    QString getName() const;

    void setSize(qint64);
    qint64 getSize() const;

private:
    AbstractNode *parent_;
    QString name_;
    qint64 size_;
};

#endif // ABSTRACTNODE_H
