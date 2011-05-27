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

void TreeManager::clear()
{
	_tree->clear();
	emit treeUpdated();
}

void TreeManager::buildTree()
{
	_tree->buildTree();
	emit treeUpdated();
}

void TreeManager::setRootPath(const QString &path)
{
	_tree->setRootPath(path);
	_tree->buildTree();
	emit treeUpdated();
}
