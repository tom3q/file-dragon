/**
  * Holds file-related information.
  */

#include <QFileInfo>
#include "filenode.h"

FileNode::FileNode()
{
    ext_ = "";
}

/**
  * When file name is set, extension is also remembered.
  */
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
