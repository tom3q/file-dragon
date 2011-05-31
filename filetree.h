/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	filetree.h
*	FileTree class prototype.
*/

#ifndef FILETREE_H
#define FILETREE_H

#include <QObject>

class DirectoryNode;
class FileFilter;

class FileTree : public QObject
{
	Q_OBJECT

public:
	FileTree();
	~FileTree();

	void clear();
	void setFilter(FileFilter &);
	void filter();
	bool isEmpty() const;

	DirectoryNode *getRoot() const;

	void printFiles();
	void setRootPath(const QString &);

private: 
	void filterDir(DirectoryNode *);
	void printFiles(DirectoryNode *);

	DirectoryNode *root_;
	QString rootPath;
	FileFilter *filter_;
};

#endif // FILETREE_H
