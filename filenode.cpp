/**
  * Holds file-related information.
  */

#include <QFileInfo>
#include "filenode.h"

FileNode::FileNode()
{
    ext_ = "";
}

FileNode::FileNode(const QFileInfo &info)
{
	AbstractNode::setName(info.absoluteFilePath());
	setSize(info.size());
	ext_ = info.completeSuffix();
	modified_ = info.lastModified();
}

/**
  * When file name is set, extension is also remembered.
  */
void FileNode::setName(const QString &name)
{
    AbstractNode::setName(name);
    QFileInfo info(name);
    ext_ = info.completeSuffix();
}

QString FileNode::getExtension() const
{
    return ext_;
}

void FileNode::setModified(QDateTime time)
{
	modified_ = time;
}

QDateTime FileNode::getModified() const
{
	return modified_;
}
