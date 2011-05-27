#include <iostream>
#include <QDir>

#include "filetree.h"
#include "directorynode.h"
#include "filefilter.h"

using namespace std;

FileTree::FileTree()
{
    root_ = 0;
	filter_ = new FileFilter();
}

FileTree::~FileTree()
{
    clear();
	delete filter_;
}

/**
  * Constructs a directory tree, beginning with a directory specified by path.
    Emits treeUpdated() signal.
  * @param path Root directory for the tree.
  */
void FileTree::buildTree()
{
    clear();
    root_ = createDir(rootPath);
	filterDir(root_);
}

void FileTree::setRootPath(const QString &path)
{
	rootPath = path;
}

/**
  * Removes all nodes from the tree. Emits treeUpdated() signal.
  */
void FileTree::clear()
{
    clear(root_);
}

void FileTree::clear(DirectoryNode *dir)
{
    if (root_ == 0) return;

    for (unsigned int i=0; i<dir->files_.size(); i++)
        delete dir->files_[i];
    for (int i=0; i<dir->getDirCount(); i++)
        clear(dir->getDir(i));
}

/**
  *	Sets a file filter for the file tree. Filtration hides files,
	which do not match filter criteria. Emits treeUpdated signal.
  */
void FileTree::setFilter(FileFilter &f)
{
	*filter_ = f;

	if (root_ != 0)
	{
		filterDir(root_);
	}
}

/**
  * Creates a directory node together with all its file
    and directory nodes. Used in "buildTree" method.
  */
DirectoryNode *FileTree::createDir(const QString &path)
{
    QDir currentDir(path);
    double sizeSum = 0;

    DirectoryNode *newNode = new DirectoryNode();
    newNode->setName(path);

    currentDir.setFilter( QDir::Files );
    QFileInfoList entries = currentDir.entryInfoList();
	for (int i = 0; i < entries.size(); ++i)
    {
		const QFileInfo &info = entries.at(i);
        FileNode *fileNode = new FileNode(info);
        sizeSum += info.size();

        newNode->addFile(fileNode);
    }

    QFileInfo currentInfo;
    currentInfo.setFile(path);

    currentDir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );
    QFileInfoList dirEntries = currentDir.entryInfoList();
    for (int i = 0; i < dirEntries.size(); ++i)
    {
		const QFileInfo &info = dirEntries.at(i);

        if (info == currentInfo)
            continue;

		DirectoryNode *dirNode = createDir(info.absoluteFilePath());
        sizeSum += dirNode->getSize();

        newNode->addDir(dirNode);
    }

    newNode->setSize(sizeSum);
    return newNode;
}

/**
  * Filters files in the tree using the given file filter.
    Unfiltered files are held in DirectoryNodes unfilteredFiles_
    vector. During the filtration, directory sizes are updated
    (they don't take "filtered" nodes into account). Emits
    treeUpdated() signal.
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
    return root_ == 0;
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
