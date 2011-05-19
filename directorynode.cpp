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
    file->setParent(this);
}

FileNode *DirectoryNode::getFile(int index) const
{
    return files_[index];
}

int DirectoryNode::getFileCount() const
{
    return files_.size();
}
