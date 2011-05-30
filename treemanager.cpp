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
#include "osoperations.h"
#include <cassert>

TreeManager::TreeManager(FileTree &tree)
	: _tree(&tree)
{
	start();
	QObject::moveToThread(this);
}

TreeManager::~TreeManager()
{
	cancel();
	this->quit();
	this->wait();
}

void TreeManager::clear()
{
	shouldCancel = false;
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
	shouldCancel = false;
	_tree->clear();
	DirectoryNode *root = _tree->getRoot();
	totalUsed = OSOperations::getUsedSpace(root->getName());
	assert(totalUsed != 0);
	totalProcessed = 0;

	emit progressUpdated(0);
	emit nowScanning(root->getName());

	scanDir(root);

	if (shouldCancel)
		_tree->clear();

	_tree->filter();

	emit progressUpdated(100);
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

	currentDir.setFilter( QDir::Files | QDir::NoSymLinks );
    QFileInfoList entries = currentDir.entryInfoList();
	for (int i = 0; i < entries.size(); ++i)
    {
		const QFileInfo &info = entries.at(i);

        FileNode *fileNode = new FileNode(info);
        sizeSum += info.size();

		totalProcessed += info.size();
		emit progressUpdated((100 * totalProcessed) / totalUsed);

        dir->addFile(fileNode);

		if (shouldCancel)
			return;
    }

    QFileInfo currentInfo;
    currentInfo.setFile(path);

	currentDir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks );
    QFileInfoList dirEntries = currentDir.entryInfoList();
    for (int i = 0; i < dirEntries.size(); ++i)
    {
		const QFileInfo &info = dirEntries.at(i);

        if (info == currentInfo)
            continue;

		DirectoryNode *dirNode = dir->addDir(info.absoluteFilePath());
		emit nowScanning(info.absoluteFilePath());
		scanDir(dirNode);
        sizeSum += dirNode->getSize();

		if (shouldCancel)
			return;
    }

    dir->setSize(sizeSum);
}

void TreeManager::setRootPath(const QString &path)
{
	shouldCancel = false;
	_tree->setRootPath(path);
}

void TreeManager::cancel()
{
	shouldCancel = true;
}
