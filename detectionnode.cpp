#include "detectionnode.h"

DetectionNode::DetectionNode(QRectF rect, FileNode *file) :
	rect_(rect), file_(file)
{
}

DetectionNode::~DetectionNode()
{
	vector<DetectionNode *>::iterator it;
	for (it = children_.begin(); it != children_.end(); ++it)
		delete (*it);
}

bool DetectionNode::contains(int x, int y)
{
	return rect_.contains(x, y);
}

QRectF DetectionNode::getRect() const
{
	return rect_;
}

FileNode *DetectionNode::getFile()
{
	return file_;
}

int DetectionNode::childrenCount()
{
	return children_.size();
}

DetectionNode *DetectionNode::child(int index)
{
	return children_[index];
}

void DetectionNode::addChild(DetectionNode *child)
{
	children_.push_back(child);
}
