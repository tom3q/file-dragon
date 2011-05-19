#include "filefilter.h"

FileFilter::FileFilter()
{
    sizeMin_ = 0;
    sizeMax_ = 0;
}

void FileFilter::setMaximumSize(long s)
{
    sizeMax_ = s;
}

long FileFilter::getMaximumSize() const
{
    return sizeMax_;
}

void FileFilter::setMinimumSize(long s)
{
    sizeMin_ = s;
}

long FileFilter::getMinimumSize() const
{
    return sizeMin_;
}

bool FileFilter::checkFile(FileNode *file)
{
    long fileSize = file->getSize();

    if (fileSize < sizeMin_)
        return false;
    if (sizeMax_ != 0 && fileSize > sizeMax_)
        return false;

    return true;
}
