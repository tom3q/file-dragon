#ifndef FILETREE_H
#define FILETREE_H

#include <QObject>
#include <set>

using namespace std;

class DirectoryNode;
class FileNode;
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
	void setMarkedFiles(set<FileNode*> &s);

	void printFiles();
	void setRootPath(const QString &);

private: 
	void filterDir(DirectoryNode *);
	void printFiles(DirectoryNode *);

	set<FileNode*> markedFiles_;
	DirectoryNode *root_;
	QString rootPath;
	FileFilter *filter_;
};

#endif // FILETREE_H
