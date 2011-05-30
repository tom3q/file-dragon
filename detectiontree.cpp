#include "cassert"
#include "detectiontree.h"

#include "QDebug"

DetectionTree::DetectionTree()
{
	root_ = 0;
}

DetectionTree::~DetectionTree()
{
	if (root_)
		delete root_;
}

FileNode *DetectionTree::detectFile(int x, int y)
{
	DetectionNode *dNode = root_;

	if (root_ == 0) return 0;

	while (dNode->getFile() == 0)
	{
		for (int i=0; i<dNode->childrenCount(); ++i)
		{
			if (dNode->child(i)->contains(x, y))
			{
				dNode = dNode->child(i);
				break;
			}
		}
	}

	return dNode->getFile();
}

DirectoryNode *DetectionTree::detectDirectory(int x, int y)
{
	AbstractNode *node;
	DetectionNode *dNode = root_;
	int level = 0;

	if (root_ == 0) return 0;

	while (dNode->getFile() == 0)
	{
		for (int i=0; i<dNode->childrenCount(); ++i)
		{
			if (dNode->child(i)->contains(x, y))
			{
				dNode = dNode->child(i);
				level++;
				break;
			}
		}
	}

	if (level > 1)
	{
		node = dNode->getFile();
		for (int i=1; i<level; i++)
			node = node->getParent();
		return (DirectoryNode *) node;
	}
	else
	{
		return (DirectoryNode *) node->getParent();
	}
}

DetectionNode *DetectionTree::getRoot()
{
	return root_;
}

void DetectionTree::setRoot(QRectF rect)
{
	if (root_)
		delete root_;
	root_ = new DetectionNode(rect);
}

void DetectionTree::clear()
{
	if (root_)
	{
		delete root_;
		root_ = 0;
		rectMap_.clear();
	}
}

void DetectionTree::addChild(DetectionNode *node, DetectionNode *child)
{
	assert(root_ != 0);
	node->addChild(child);
}
