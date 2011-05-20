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

    void buildTree(QString);
    void clear();
    void filter(FileFilter &);
    bool isEmpty() const;

    DirectoryNode *getRoot() const;

    void printFiles();

signals:
    void treeUpdated();

private: 
    DirectoryNode *createDir(QString);
    void clear(DirectoryNode *);
    void filter(DirectoryNode *, FileFilter &);
    void printFiles(DirectoryNode *);

    DirectoryNode *root_;
};

#endif // FILETREE_H
