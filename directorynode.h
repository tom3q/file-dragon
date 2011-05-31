/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	directorynode.h
*	DirectoryNode class prototype.
*/

#ifndef DIRECTORYNODE_H
#define DIRECTORYNODE_H

#include <vector>

#include "abstractnode.h"
#include "filenode.h"
#include "filetree.h"

/**
 * DirectoryNode holds typical directory information - name, size,
 * its files and subdirs. Files are located in two vector objects:
 * files_ and unfilteredFiles_. First container stores real
 * directory structure. unfilteredFiles_ holds only files which
 * weren't filtered out during the filtration process. From the
 * user side, only unfiltered files are accessible. The filtration
 * process is done by a friend class FileTree.
 */
class DirectoryNode : public AbstractNode
{
public:
	/**
	 * Destructor clearing recursively the whole subtree.
	 */
	~DirectoryNode()
	{
		clear();
	}
	/**
	 * Adds subdirectory with given name to the tree.
	 * @param name Name of subdirectory to add.
	 * @return Pointer to DirectoryNode created in the operation.
	 */
	DirectoryNode *addDir(const QString &name);
	/**
	 * Returns a pointer to DirectoryNode of index'th subdirectory.
	 * @param index Subdirectory index.
	 * @return Pointer to DirectoryNode of requested subdirectory.
	 */
	inline DirectoryNode *getDir(int index) const
	{
		return dirs_[index];
	}
	/**
	 * Returns the number of subdirectories.
	 * @return Number of subdirectories.
	 */
	inline int getDirCount() const
	{
		return dirs_.size();
	}
	/**
	 * Adds file with given name to the tree.
	 * @param file FileNode describing the file.
	 */
	void addFile(FileNode *file);
	/**
	 * Returns a pointer to FileNode of index'th file in the directory.
	 * @param index File index.
	 * @return Pointer to FileNode of requested file.
	 */
	inline FileNode *getFile(int index) const
	{
		return unfilteredFiles_[index];
	}
	/**
	 * Returns the number of files.
	 * @return Number of files.
	 */
	inline int getFileCount() const
	{
		return unfilteredFiles_.size();
	}

	/**
	 * Empties the directory.
	 */
	void clear();
	/**
	 * Checks whether the directory is empty.
	 * @return True if the directory is empty.
	 */
	inline bool empty() const
	{
		return files_.empty() && dirs_.empty();
	}

	/**
	 * Checks whether the node is a directory.
	 * @return Always true for DirectoryNode.
	 */
	inline bool isDir() const
	{
		return true;
	}

private:
	/*
	 * A functor template for freeing all elements of a container
	 */
	template<typename T>
	class DeleteIt
	{
	public:
		void operator()(T *t)
		{
			delete t;
		}
	};

	friend class FileTree;

	std::vector<DirectoryNode*> dirs_;
	std::vector<FileNode*> files_;
	std::vector<FileNode*> unfilteredFiles_;
};

#endif // DIRECTORYNODE_H
