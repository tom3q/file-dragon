#ifndef FILENODE_H
#define FILENODE_H

#include <QDateTime>

#include "abstractnode.h"
#include "filetree.h"

#include <QFileInfo>

class FileNode : public AbstractNode
{
public:
	FileNode();
	FileNode(const QFileInfo &info);

	void setName(const QString &);

	inline void setMarked(bool mark)
	{
		marked_ = mark;
	}
	inline bool isMarked() const
	{
		return marked_;
	}

	inline QString getExtension() const
	{
		return ext_;
	}

	inline void setModified(QDateTime time)
	{
		modified_ = time;
	}
	inline QDateTime getModified() const
	{
		return modified_;
	}

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
