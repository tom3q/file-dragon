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

	inline DirectoryNode *getDir(int index) const
	{
	    return dirs_[index];
	}

	inline int getDirCount() const
	{
		return dirs_.size();
	}

	void addFile(FileNode *);

	inline FileNode *getFile(int index) const
	{
		return unfilteredFiles_[index];
	}

	inline int getFileCount() const
	{
		return unfilteredFiles_.size();
	}

	void clear();

	inline bool empty() const
	{
		return files_.empty() && dirs_.empty();
	}

	inline bool isDir() const
	{
		return true;
	}

private:
    friend class FileTree;

    vector<DirectoryNode*> dirs_;
    vector<FileNode*> files_;
    vector<FileNode*> unfilteredFiles_;
};

#endif // DIRECTORYNODE_H
