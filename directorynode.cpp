/**
  * DirectoryNode holds typical directory information - name, size,
    its files and subdirs. Files are located in two vector objects:
    files_ and unfilteredFiles_. First container stores real
    directory structure. unfilteredFiles_ holds only files which
    weren't filtered out during the filtration process. From the
    user side, only unfiltered files are accessible. The filtration
    process is done by a friend class FileTree.
  */

#include <cassert>
#include "directorynode.h"

DirectoryNode::DirectoryNode()
{
}

DirectoryNode *DirectoryNode::addDir(const QString &name)
{
	DirectoryNode *node = new DirectoryNode();
    dirs_.push_back(node);
    node->setParent(this);
	node->setName(name);
	return node;
}

DirectoryNode *DirectoryNode::getDir(int index) const
{
    return dirs_[index];
}

int DirectoryNode::getDirCount() const
{
    return dirs_.size();
}

void DirectoryNode::addFile(FileNode *file)
{
    files_.push_back(file);
    unfilteredFiles_.push_back(file);
    file->setParent(this);
}

FileNode *DirectoryNode::getFile(int index) const
{
    return unfilteredFiles_[index];
}

int DirectoryNode::getFileCount() const
{
    return unfilteredFiles_.size();
}

void DirectoryNode::clear()
{
    for (unsigned int i=0; i<files_.size(); ++i)
        delete files_[i];

	files_.clear();

    for (int i = 0; i < getDirCount(); ++i) {
		DirectoryNode *d = getDir(i);

		assert(d != 0);
        d->clear();

		delete d;
	}

	dirs_.clear();
}
