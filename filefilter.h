/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	filefilter.h
*	FileFilter class prototype.
*/

#ifndef FILEFILTER_H
#define FILEFILTER_H

#include "filenode.h"

/**
 * Class used to filter files in a FileTree class. In order to
 * filter files, call FileTree::filter(FileFilter). FileFilter
 * class holds parameters used during filtration (eg. file size)
 * and uses checkFile(FileNode*) method to determine if a given
 * file "passes" the selection or not.
 */
class FileFilter
{
public:
	/**
	 * Default constructor
	 */
	FileFilter();

	/**
	 * Sets maximum file size accepted by filter.
	 * @param max Maximum file size.
	 */
	void setMaximumSize(qint64 max);
	/**
	 * Gets maximum file size accepted by filter.
	 * @return Maximum file size.
	 */
	qint64 getMaximumSize() const;

	/**
	 * Sets minimum file size accepted by filter.
	 * @param min Minimum file size.
	 */
	void setMinimumSize(qint64 min);
	/**
	 * Gets minimum file size accepted by filter.
	 * @return Minimum file size.
	 */
	qint64 getMinimumSize() const;

	/**
	 * Checks if a file passes the filtration rules.
	 * @param file FileNode pointer.
	 * @return True if passes, False if not.
	 */
	bool checkFile(FileNode *);

private:
	qint64 sizeMin_, sizeMax_;
};

#endif // FILEFILTER_H
