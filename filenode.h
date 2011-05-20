#ifndef FILENODE_H
#define FILENODE_H

#include "abstractnode.h"
#include "filetree.h"

class FileNode : public AbstractNode
{
public:
    FileNode();

    void setName(QString);
    QString getExtension() const;

private:
    QString ext_;
};

#endif // FILENODE_H
