/*
 *	FILE DRAGON
 *	A disk space management utility.
 *
 *	Developed by:
 *	-> Tomasz Figa
 *	-> Piotr T¹kiel
 *
 *	treemanager.cpp
 *	TreeManager class definitions.
 */

#include "treemanager.h"

TreeManager::TreeManager(FileTree &tree)
	: _tree(&tree)
{
	start();
	QObject::moveToThread(this);
}

TreeManager::~TreeManager()
{
	this->quit();
	this->wait();
}

void TreeManager::clear()
{
	_tree->clear();
	emit treeUpdated();
}

/**
  * Constructs a directory tree, beginning with a directory specified by path.
    Emits treeUpdated() signal.
  * @param path Root directory for the tree.
  */
void TreeManager::buildTree()
{
	_tree->clear();
	scanDir(_tree->getRootNode());
	emit treeUpdated();
}

/**
  * Creates a directory node together with all its file
    and directory nodes. Used in "buildTree" method.
  */
void TreeManager::scanDir(DirectoryNode *dir)
{
	QString path(dir->getName());
    QDir currentDir(path);
    double sizeSum = 0;

    currentDir.setFilter( QDir::Files );
    QFileInfoList entries = currentDir.entryInfoList();
	for (int i = 0; i < entries.size(); ++i)
    {
		const QFileInfo &info = entries.at(i);

        FileNode *fileNode = new FileNode(info);
        sizeSum += info.size();

        dir->addFile(fileNode);
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

		DirectoryNode *dirNode = dir->addDir(info.absoluteFilePath());
		scanDir(dirNode);
        sizeSum += dirNode->getSize();
    }

    dir->setSize(sizeSum);
}

void TreeManager::setRootPath(const QString &path)
{
	_tree->setRootPath(path);
}
