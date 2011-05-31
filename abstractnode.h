/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	abstractnode.h
*	AbstractNode class prototype.
*/

#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QString>

/**
 * AbstractNode is a class from which DirectoryNode and FileNode
 * inherit. This class 2 attributes - size and name - both used
 * by subclasses.
 */
class AbstractNode
{
public:
	/**
	 * Default constructor
	 */
	AbstractNode();
	/**
	 * Sets node parent.
	 * @param parent New parent for node.
	 */
	inline void setParent(AbstractNode *parent)
	{
		this->parent_ = parent;
	}
	/**
	 * Gets node parent.
	 * @returns Node parent.
	 */
	inline AbstractNode *getParent() const
	{
		return parent_;
	}

	/**
	 * Sets node name.
	 * @param name New node name.
	 */
	inline void setName(const QString &name)
	{
		name_ = name;
	}
	/**
	 * Gets node name.
	 * @returns Node name.
	 */
	inline QString getName() const
	{
		return name_;
	}

	/**
	 * Sets node size.
	 * @param size New node size.
	 */
	inline void setSize(qint64 size)
	{
		size_ = size;
	}
	/**
	 * Gets node size.
	 * @returns Node size.
	 */
	inline qint64 getSize() const
	{
		return size_;
	}

	/**
	 * Checks whether the node is a directory.
	 * @returns True if the node is a directory.
	 */
	virtual bool isDir() const = 0;

private:
	AbstractNode *parent_;
	QString name_;
	qint64 size_;
};

#endif // ABSTRACTNODE_H
