#include <iostream>
#include <QDir>

#include "filetree.h"
#include "directorynode.h"
#include "filefilter.h"

using namespace std;

FileTree::FileTree()
{
    root_ = 0;
}

FileTree::~FileTree()
{
    clear();
}

/**
  * Constructs a directory tree, beginning with a directory specified by path.
    Emits treeUpdated() signal.
  * @param path Root directory for the tree.
  */
void FileTree::buildTree(QString path)
{
    clear();
    root_ = createDir(path);
    emit treeUpdated();
}

/**
  * Removes all nodes from the tree. Emits treeUpdated() signal.
  */
void FileTree::clear()
{
    clear(root_);
    emit treeUpdated();
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
  * Creates a directory node together with all its file
    and directory nodes. Used in "buildTree" method.
  */
DirectoryNode *FileTree::createDir(QString path)
{
    QFileInfo info;
    QDir currentDir(path);
    double sizeSum = 0;

    DirectoryNode *newNode = new DirectoryNode();
    newNode->setName(path);

    currentDir.setFilter( QDir::Files );
    QStringList entries = currentDir.entryList();
    for (int i=0; i<entries.size(); i++)
    {
        info.setFile(path, entries.at(i));

        FileNode *fileNode = new FileNode();
        fileNode->setName(info.absoluteFilePath());
        fileNode->setSize(info.size());
        sizeSum += fileNode->getSize();

        newNode->addFile(fileNode);
    }

    QFileInfo currentInfo;
    currentInfo.setFile(path);

    currentDir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );
    entries = currentDir.entryList();
    for (int i=0; i<entries.size(); i++)
    {
        info.setFile(path, entries.at(i));
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
  * @param fileFilter Filter object used to filter files.
  */
void FileTree::filter(FileFilter &fileFilter)
{
    if (root_ == 0) return;
    filter(root_, fileFilter);
    emit treeUpdated();
}

void FileTree::filter(DirectoryNode *currentDir, FileFilter &fileFilter)
{
    DirectoryNode *dir;
    FileNode *file;
    double sizeSum = 0;

    currentDir->unfilteredFiles_.clear();

    for (unsigned int i=0; i<currentDir->files_.size(); i++)
    {
        file = currentDir->files_[i];
        if (fileFilter.checkFile(file))
        {
            currentDir->unfilteredFiles_.push_back(file);
            sizeSum += file->getSize();
        }
    }
    for (int i=0; i<currentDir->getDirCount(); i++)
    {
        dir = currentDir->getDir(i);
        filter(dir, fileFilter);
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
