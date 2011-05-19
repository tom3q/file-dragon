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

    void setFiltered(bool);
    bool isFiltered() const;

private:
    QString ext_;
    bool filtered_;
};

#endif // FILENODE_H
