/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	filefilter.cpp
*	FileFilter class implementation.
*/

#include "filefilter.h"

FileFilter::FileFilter()
{
	sizeMin_ = 0;
	sizeMax_ = 0;
}

void FileFilter::setMaximumSize(qint64 s)
{
	sizeMax_ = s;
}

qint64 FileFilter::getMaximumSize() const
{
	return sizeMax_;
}

void FileFilter::setMinimumSize(qint64 s)
{
	sizeMin_ = s;
}

qint64 FileFilter::getMinimumSize() const
{
	return sizeMin_;
}

bool FileFilter::checkFile(FileNode *file)
{
	qint64 fileSize = file->getSize();

	if (fileSize < sizeMin_)
		return false;
	if (sizeMax_ != 0 && fileSize > sizeMax_)
		return false;

	return true;
}
