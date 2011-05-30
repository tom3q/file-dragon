#ifndef FILEFILTER_H
#define FILEFILTER_H

#include "filenode.h"

class FileFilter
{
public:
    FileFilter();

    void setMaximumSize(double);
    double getMaximumSize() const;

    void setMinimumSize(double);
    double getMinimumSize() const;

    bool checkFile(FileNode *);

private:
    double sizeMin_, sizeMax_;
};

#endif // FILEFILTER_H
