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
    QString getExtension() const;

	void setModified(QDateTime);
	QDateTime getModified() const;

	bool isDir() {return false;}

private:
    QString ext_;
	QDateTime modified_;
};

#endif // FILENODE_H
