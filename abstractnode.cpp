/**
  * AbstractNode is a class from which DirectoryNode and FileNode
    inherit. This class 2 attributes - size and name - both used
    by subclasses.
  */

#include "abstractnode.h"

AbstractNode::AbstractNode()
{
    parent_ = 0;
    size_ = 0;
}


