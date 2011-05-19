#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QString>

class AbstractNode
{
public:
    AbstractNode();

    void setParent(AbstractNode *);
    AbstractNode *getParent() const;

    void setName(QString);
    QString getName() const;

    void setSize(long);
    long getSize() const;

private:
    AbstractNode *parent_;
    QString name_;
    long size_;
};

#endif // ABSTRACTNODE_H
