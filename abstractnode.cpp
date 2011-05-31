/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T�kiel
*
*	abstractnode.cpp
*	AbstractNode class definition.
*/

#include "abstractnode.h"

AbstractNode::AbstractNode()
{
	parent_ = 0;
	size_ = 0;
}
