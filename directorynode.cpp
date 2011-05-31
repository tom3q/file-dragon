/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	directorynode.cpp
*	DirectoryNode class implementation.
*/

#include <cassert>
#include <algorithm>
#include "directorynode.h"
#include "filenode.h"

using namespace std;

DirectoryNode *DirectoryNode::addDir(const QString &name)
{
	DirectoryNode *node = new DirectoryNode();
	dirs_.push_back(node);
	node->setParent(this);
	node->setName(name);
	return node;
}

void DirectoryNode::addFile(FileNode *file)
{
	files_.push_back(file);
	unfilteredFiles_.push_back(file);
	file->setParent(this);
}

void DirectoryNode::clear()
{
	for_each(files_.cbegin(), files_.cend(), DeleteIt<FileNode>());
	files_.clear();
	unfilteredFiles_.clear();

	for_each(dirs_.cbegin(), dirs_.cend(), DeleteIt<DirectoryNode>());
	dirs_.clear();
}
