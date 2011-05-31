/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	filenode.h
*	FileNode class prototype.
*/

#ifndef FILENODE_H
#define FILENODE_H

#include <QDateTime>
#include <QFileInfo>

#include "abstractnode.h"
#include "filetree.h"

/**
 * FileNode is a class for holding file-related information.
 */
class FileNode : public AbstractNode
{
public:
	/**
	 * Trivial default constructor.
	 */
	FileNode() : marked_(false), ext_("")  {};
	/**
	 * Constructor for conversion from QFileInfo.
	 */
	FileNode(const QFileInfo &info);

	inline void setMarked(bool mark)
	{
		marked_ = mark;
	}
	inline bool isMarked() const
	{
		return marked_;
	}

	/**
	 * Sets file name.
	 * @param name New file name.
	 */
	void setName(const QString &name);

	/**
	 * Returns file extension.
	 * @return File extension.
	 */
	inline QString getExtension() const
	{
		return ext_;
	}

	/**
	 * Sets file modification time.
	 * @param time New modification time.
	 */
	inline void setModified(QDateTime time)
	{
		modified_ = time;
	}
	/**
	 * Gets file modification time.
	 * @return File modification time.
	 */
	inline QDateTime getModified() const
	{
		return modified_;
	}

	/**
	 * Checks whether the node is a directory.
	 * @return Always false for FileNode.
	 */
	inline bool isDir() const
	{
		return false;
	}

private:
	bool marked_;
	QString ext_;
	QDateTime modified_;
};

#endif // FILENODE_H
