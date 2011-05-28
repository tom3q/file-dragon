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

	DirectoryNode *addDir(const QString &);
	void clear();
    DirectoryNode *getDir(int) const;
    int getDirCount() const;

    void addFile(FileNode *);
    FileNode *getFile(int) const;
    int getFileCount() const;
	bool empty() const;

private:
    friend class FileTree;

    vector<DirectoryNode*> dirs_;
    vector<FileNode*> files_;
    vector<FileNode*> unfilteredFiles_;
};

#endif // DIRECTORYNODE_H
