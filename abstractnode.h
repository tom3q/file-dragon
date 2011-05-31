#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QString>

class AbstractNode
{
public:
    AbstractNode();

	inline void setParent(AbstractNode *parent)
	{
		this->parent_ = parent;
	}
	inline AbstractNode *getParent() const
	{
		return parent_;
	}

	inline void setName(const QString &name)
	{
		name_ = name;
	}
	inline QString getName() const
	{
		return name_;
	}

	inline void setSize(qint64 size)
	{
		size_ = size;
	}
	inline qint64 getSize() const
	{
		return size_;
	}

	virtual bool isDir() const = 0;

private:
    AbstractNode *parent_;
    QString name_;
    qint64 size_;
};

#endif // ABSTRACTNODE_H
