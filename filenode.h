#ifndef FILENODE_H
#define FILENODE_H

#include <QDateTime>

#include "abstractnode.h"
#include "filetree.h"

class FileNode : public AbstractNode
{
public:
    FileNode();

    void setName(QString);
    QString getExtension() const;

	void setModified(QDateTime);
	QDateTime getModified() const;

private:
    QString ext_;
	QDateTime modified_;
};

#endif // FILENODE_H
