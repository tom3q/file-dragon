#ifndef FILETREE_H
#define FILETREE_H

#include <QObject>
//#include "directorynode.h"
//#include "filefilter.h"

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
    bool isEmpty() const;

    DirectoryNode *getRoot() const;

    void printFiles();
	void setRootPath(const QString &);
	DirectoryNode *getRootNode();

private: 
	void filterDir(DirectoryNode *);
    void printFiles(DirectoryNode *);

    DirectoryNode *root_;
	QString rootPath;
	FileFilter *filter_;
};

#endif // FILETREE_H
