/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	filetree.cpp
*	FileTree class implementation.
*/

#include <iostream>
#include <QDir>
#include <cassert>

#include "filetree.h"
#include "directorynode.h"
#include "filefilter.h"

using namespace std;

FileTree::FileTree()
{
	root_ = new DirectoryNode();
	filter_ = new FileFilter();
}

FileTree::~FileTree()
{
	clear();
	delete root_;
	delete filter_;
}

void FileTree::setRootPath(const QString &path)
{
	root_->setName(path);
}

/**
 * Removes all nodes from the tree. Emits treeUpdated() signal.
 */
void FileTree::clear()
{
	assert(root_ != 0);
	root_->clear();
}

/**
 * Sets a file filter for the file tree. Filtration hides files,
 * which do not match filter criteria. Emits treeUpdated signal.
 */
void FileTree::setFilter(FileFilter &f)
{
	*filter_ = f;

	assert(root_ != 0);
	filterDir(root_);
}

void FileTree::filter()
{
	assert(root_ != 0);
	filterDir(root_);
}

/**
 * Filters files in the tree using the given file filter.
 * Unfiltered files are held in DirectoryNodes unfilteredFiles_
 * vector. During the filtration, directory sizes are updated
 * (they don't take "filtered" nodes into account). Emits
 * treeUpdated() signal.
 * @param currentDir A directory to be filtered.
 * @param fileFilter Filter object used to filter files.
 */
void FileTree::filterDir(DirectoryNode *currentDir)
{
	DirectoryNode *dir;
	FileNode *file;
	double sizeSum = 0;

	currentDir->unfilteredFiles_.clear();

	for (unsigned int i=0; i<currentDir->files_.size(); i++)
	{
		file = currentDir->files_[i];
		if (filter_->checkFile(file))
		{
			currentDir->unfilteredFiles_.push_back(file);
			sizeSum += file->getSize();
		}
	}
	for (int i=0; i<currentDir->getDirCount(); i++)
	{
		dir = currentDir->getDir(i);
		filterDir(dir);
		sizeSum += dir->getSize();
	}

	currentDir->setSize(sizeSum);
}

bool FileTree::isEmpty() const
{
	return root_->empty();
}

DirectoryNode *FileTree::getRoot() const
{
	return root_;
}

/**
 * Prints out files in the tree.
 */
void FileTree::printFiles()
{
	printFiles(root_);
}

void FileTree::printFiles(DirectoryNode *dir)
{
	for (int i=0; i<dir->getFileCount(); i++)
		cout << dir->getFile(i)->getName().toStdString() << endl;
	for (int i=0; i<dir->getDirCount(); i++)
		printFiles(dir->getDir(i));
}
