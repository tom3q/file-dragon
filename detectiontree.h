#ifndef DETECTIONTREE_H
#define DETECTIONTREE_H

#include <map>

#include "detectionnode.h"
#include "filenode.h"
#include "directorynode.h"

using namespace std;

class DetectionTree
{
public:
    DetectionTree();
	~DetectionTree();

	FileNode *detectFile(int x, int y);
	DirectoryNode *detectDirectory(int x, int y);

	DetectionNode *getRoot();
	void setRoot(QRectF rect);
	void clear();
	void addChild(DetectionNode *node, DetectionNode *child);

private:
	map<FileNode *, QRectF> rectMap_;
	DetectionNode *root_;
};

#endif // DETECTIONTREE_H
