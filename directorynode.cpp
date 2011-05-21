/**
  * DirectoryNode holds typical directory information - name, size,
    its files and subdirs. Files are located in two vector objects:
    files_ and unfilteredFiles_. First container stores real
    directory structure. unfilteredFiles_ holds only files which
    weren't filtered out during the filtration process. From the
    user side, only unfiltered files are accessible. The filtration
    process is done by a friend class FileTree.
  */

#include "directorynode.h"

DirectoryNode::DirectoryNode()
{
}

void DirectoryNode::addDir(DirectoryNode *node)
{
    dirs_.push_back(node);
    node->setParent(this);
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
