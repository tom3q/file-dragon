/**
  * Class used to filter files in a FileTree class. In order to
    filter files, call FileTree::filter(FileFilter). FileFilter
    class holds parameters used during filtration (eg. file size)
    and uses checkFile(FileNode*) method to determine if a given
    file "passes" the selection or not.
  */

#include "filefilter.h"

FileFilter::FileFilter()
{
    sizeMin_ = 0;
    sizeMax_ = 0;
}

void FileFilter::setMaximumSize(double s)
{
    sizeMax_ = s;
}

double FileFilter::getMaximumSize() const
{
    return sizeMax_;
}

void FileFilter::setMinimumSize(double s)
{
    sizeMin_ = s;
}

double FileFilter::getMinimumSize() const
{
    return sizeMin_;
}

/**
  * Checks if a file passes the filtration rules.
  * @param file FileNode pointer.
  * @return True if passes, False if not.
  */
bool FileFilter::checkFile(FileNode *file)
{
    double fileSize = file->getSize();

    if (fileSize < sizeMin_)
        return false;
    if (sizeMax_ != 0 && fileSize > sizeMax_)
        return false;

    return true;
}
