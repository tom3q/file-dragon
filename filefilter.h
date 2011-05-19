#ifndef FILEFILTER_H
#define FILEFILTER_H

#include "filenode.h"

class FileFilter
{
public:
    FileFilter();

    void setMaximumSize(long);
    long getMaximumSize() const;

    void setMinimumSize(long);
    long getMinimumSize() const;

    bool checkFile(FileNode *);

private:
    long sizeMin_, sizeMax_;
};

#endif // FILEFILTER_H
