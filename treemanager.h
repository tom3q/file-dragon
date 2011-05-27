/*
 *	FILE DRAGON
 *	A disk space management utility.
 *
 *	Developed by:
 *	-> Tomasz Figa
 *	-> Piotr T¹kiel
 *
 *	treemanager.h
 *	TreeManager class prototype.
 */

#ifndef _TREEMANAGER_H_
#define _TREEMANAGER_H_

#include <QString>
#include <QThread>
#include "filetree.h"

/**
 *	Tree manager class.
 *	A class to manage a file tree in separate thread.
 */
class TreeManager :	public QThread
{
	Q_OBJECT

public:
	/**
	 *	Object constructor.
	 *	@param tree The tree to be managed.
	 */
	TreeManager(FileTree &tree);

signals:
	/**
	 *	Tree updated signal.
	 *	Signals that the tree has been updated.
	 */
    void treeUpdated();

public slots:
	/**
	 *	Clear slot.
	 *	Clears the tree and emits treeUpdated().
	 */
	void clear();
	/**
	 *	Build tree slot.
	 *	Rebuilds the tree and emits treeUpdated().
	 */
	void buildTree();
	/**
	 *	Set root path slot.
	 *	Sets new root path from the argument, rebuilds the tree
	 *	and then emits treeUpdated().
	 *	@param path Root path to be set.
	 */
	void setRootPath(const QString &path);

private:
	FileTree *_tree;
};

#endif
