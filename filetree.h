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

    void buildTree();
    void clear();
    void filter(FileFilter &);
    bool isEmpty() const;

    DirectoryNode *getRoot() const;

    void printFiles();
	void setRootPath(const QString &);

private: 
    DirectoryNode *createDir(const QString &);
    void clear(DirectoryNode *);
    void filter(DirectoryNode *, FileFilter &);
    void printFiles(DirectoryNode *);

    DirectoryNode *root_;
	QString rootPath;
};

#endif // FILETREE_H
