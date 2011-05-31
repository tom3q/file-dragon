/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	detectionnode.h
*	DetectionNode class prototype.
*/

#ifndef DETECTIONNODE_H
#define DETECTIONNODE_H

#include <QRectF>
#include <vector>
#include "filenode.h"

using namespace std;

class DetectionNode
{
public:
	DetectionNode(QRectF rect, FileNode *file = 0);
	~DetectionNode();

	bool contains(int x, int y);
	QRectF getRect() const;
	FileNode *getFile();

	int childrenCount();
	DetectionNode *child(int index);
	void addChild(DetectionNode *child);

private:
	QRectF rect_;
	FileNode *file_;
	vector<DetectionNode *> children_;
};

#endif // DETECTIONNODE_H
