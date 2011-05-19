#ifndef DIRECTORYNODE_H
#define DIRECTORYNODE_H

#include <vector>
#include "abstractnode.h"
#include "filenode.h"
#include "filetree.h"

using namespace std;

class DirectoryNode : public AbstractNode
{
public:
    DirectoryNode();

    void addDir(DirectoryNode *);
    DirectoryNode *getDir(int) const;
    int getDirCount() const;

    void addFile(FileNode *);
    FileNode *getFile(int) const;
    int getFileCount() const;

private:
    friend class FileTree;

    vector<DirectoryNode*> dirs_;
    vector<FileNode*> files_;
    vector<FileNode*> unfilteredFiles_;
};

#endif // DIRECTORYNODE_H
