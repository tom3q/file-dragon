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

void AbstractNode::setName(QString name)
{
    name_ = name;
}

QString AbstractNode::getName() const
{
    return name_;
}

void AbstractNode::setSize(long size)
{
    size_ = size;
}

long AbstractNode::getSize() const
{
    return size_;
}
