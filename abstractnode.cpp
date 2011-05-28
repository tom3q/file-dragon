/**
  * AbstractNode is a class from which DirectoryNode and FileNode
    inherit. This class 2 attributes - size and name - both used
    by subclasses.
  */

#include "abstractnode.h"

AbstractNode::AbstractNode()
{
    parent_ = 0;
    size_ = 0;
}

void AbstractNode::setParent(AbstractNode *parent)
{
    this->parent_ = parent;
}

AbstractNode *AbstractNode::getParent() const
{
    return parent_;
}

void AbstractNode::setName(const QString &name)
{
    name_ = name;
}

QString AbstractNode::getName() const
{
    return name_;
}

void AbstractNode::setSize(qint64 size)
{
    size_ = size;
}

qint64 AbstractNode::getSize() const
{
    return size_;
}
