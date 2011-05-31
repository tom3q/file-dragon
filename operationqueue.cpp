#include "operationqueue.h"

OperationQueue::OperationQueue()
{
	queue_.push_back( set<FileNode*>() );
	currentOp_ = queue_.begin();
}

int OperationQueue::addDeleteOperation(const set<FileNode *> &files)
{
	// add files to marked files
	set<FileNode*>::iterator it;
	set<FileNode*> added;
	for (it = files.begin(); it != files.end(); ++it)
	{
		if (markedFiles_.find(*it) == markedFiles_.end())
		{
			markedFiles_.insert(*it);
			added.insert(*it);
		}
	}

	// if no files new added, consider no operation took place
	if (!added.size())
		return 0;

	// dispose of any later operations
	list<set<FileNode*> >::iterator nextIt(currentOp_);
	nextIt++;
	queue_.erase(nextIt, queue_.end());

	// add operation to queue
	queue_.push_back(added);

	// move current iterator one step further
	currentOp_++;
	return added.size();
}

int OperationQueue::undo()
{
	if (currentOp_ == queue_.begin())
		return 0;

	// unmark files
	set<FileNode*>::iterator it;
	set<FileNode*> &operation = *currentOp_;
	int prevSize = markedFiles_.size();
	for (it = operation.begin(); it != operation.end(); ++it)
		markedFiles_.erase(*it);

	// step back
	currentOp_--;

	return prevSize - markedFiles_.size();
}

int OperationQueue::redo()
{
	list<set<FileNode*> >::iterator nextIt(currentOp_);
	if (++nextIt == queue_.end())
		return 0;

	// step further
	currentOp_++;

	// mark files again
	set<FileNode*>::iterator it;
	set<FileNode*> &operation = *currentOp_;
	int prevSize = markedFiles_.size();
	for (it = operation.begin(); it != operation.end(); ++it)
		markedFiles_.insert(*it);

	return markedFiles_.size() - prevSize;
}

void OperationQueue::clear()
{
	markedFiles_.clear();
	queue_.clear();
	queue_.push_back( set<FileNode*>() );
	currentOp_ = queue_.begin();
}

bool OperationQueue::canUndo() const
{
	return currentOp_ != queue_.begin();
}

bool OperationQueue::canRedo() const
{
	list<set<FileNode*> >::iterator nextIt(currentOp_);
	return ++nextIt != queue_.end();
}

bool OperationQueue::empty() const
{
	return markedFiles_.size() == 0;
}

const QStringList OperationQueue::getMarkedFilenames() const
{
	set<FileNode*>::const_iterator it;
	QStringList files;

	files.reserve(markedFiles_.size());
	for (it = markedFiles_.begin(); it != markedFiles_.end(); ++it)
		files.push_back((*it)->getName());

	return files;
}

const set<FileNode*> OperationQueue::getMarkedFiles() const
{
	return markedFiles_;
}
