#include <QFileInfo>
#include "filenode.h"

FileNode::FileNode()
{
    ext_ = "";
    filtered_ = false;
}

void FileNode::setName(QString name)
{
    AbstractNode::setName(name);
    QFileInfo info(name);
    ext_ = info.completeSuffix();
}

QString FileNode::getExtension() const
{
    return ext_;
}

void FileNode::setFiltered(bool b)
{
    filtered_ = b;
}

bool FileNode::isFiltered() const
{
    return filtered_;
}
