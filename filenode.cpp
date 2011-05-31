/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	filenode.cpp
*	FileNode class implementation.
*/

#include <QFileInfo>
#include "filenode.h"

FileNode::FileNode(const QFileInfo &info) :
	marked_(false)
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
