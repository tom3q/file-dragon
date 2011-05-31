#ifndef OPERATIONQUEUE_H
#define OPERATIONQUEUE_H

#include <QStringList>
#include <set>

#include "filenode.h"

using namespace std;

class OperationQueue
{
public:
    OperationQueue();

	/**
	  *	Adds an operation to a list. Operation consists of a set of files,
		which are supposed to be deleted. If given files are already in the
		queue, no operation is added to list.
	  * @param files Set of deleted files.
	  * @return Number of files added as an operation.
	  */
	int addDeleteOperation(const set<FileNode*> &files);

	/**
	  * Undoes the last opearation.
	  */
	int undo();

	/**
	  * Redoes the operation.
	  */
	int redo();

	/**
	  * Removes all operations from the list.
	  */
	void clear();

	/**
	  * Self-explainatory.
	  * @return True, if it is possible to undo the operation.
	  */
	bool canUndo() const;

	/**
	  * Self-explainatory.
	  * @return True, if it is possible to redo the operation.
	  */
	bool canRedo() const;

	/**
	  * Checks if a queue is empty.
	  * @return True, if queue is empty. False if not.
	  */
	bool empty() const;

	/**
	  * Returns a filename list of files, which are meant to be deleted.
	  * @return List of file names.
	  */
	const QStringList getMarkedFilenames() const;

	/**
	  * Returns a set of files, which are meant to be deleted.
	  * @return List of files.
	  */
	const set<FileNode*> getMarkedFiles() const;

private:
	list<set<FileNode*> >::iterator currentOp_;
	list<set<FileNode*> > queue_;
	set<FileNode *> markedFiles_;
};

#endif // OPERATIONQUEUE_H
