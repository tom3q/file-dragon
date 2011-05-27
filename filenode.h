#ifndef FILENODE_H
#define FILENODE_H

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

private:
    QString ext_;
};

#endif // FILENODE_H
