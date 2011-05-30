#include <QtGui>
#include <QDebug>
#include <QTime>

#include "treemapwidget.h"
#include "directorynode.h"

using namespace std;

TreemapWidget::TreemapWidget(QWidget *parent) :
    QWidget(parent)
{
	setAutoFillBackground(false);

    // create file tree and connect signals-slots
    tree_ = new FileTree();
	currentRoot_ = tree_->getRoot();

	// detection tree root
	detectRoot_ = 0;

    // create a cell renderer
	renderer_ = new DefaultRenderer(this);

	// don't show legend
	showLegend_ = false;
}

TreemapWidget::~TreemapWidget()
{
	if (detectRoot_)
		delete detectRoot_;
	delete renderer_;
    delete tree_;
}

void TreemapWidget::setRenderer(DefaultRenderer *cr)
{
	DefaultRenderer *ptr = renderer_;
	renderer_ = cr;

	if (ptr != 0)
		delete ptr;

	update();
}

FileTree &TreemapWidget::getFileTree() const
{
    return *tree_;
}

int TreemapWidget::getSelectedCount() const
{
	return selectedNodes_.size();
}

FileNode *TreemapWidget::getSelected(int index)
{
	set<FileNode*>::iterator it = selectedNodes_.begin();
	while (index--) ++it;
	return *it;
}

const QStringList TreemapWidget::getSelectedFiles() const
{
	set<FileNode*>::const_iterator it;
	QStringList files;

	files.reserve(selectedNodes_.size());

	for (it = selectedNodes_.begin(); it != selectedNodes_.end(); ++it)
		files.push_back((*it)->getName());

	return files;
}

void TreemapWidget::removeSelection()
{
	selectedNodes_.clear();
	update();
}

bool TreemapWidget::isSelected(FileNode *node) const
{
	return selectedNodes_.find(node) != selectedNodes_.end();
}

void TreemapWidget::setShowLegend(bool show)
{
	showLegend_ = show;
	update();
}

QSize TreemapWidget::sizeHint() const
{
	return QSize(1, 10000);
}

void TreemapWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		FileNode *file = detectFile(event->x(), event->y());

		if (file != 0)
		{
			if (QApplication::keyboardModifiers() & Qt::ControlModifier)
			{
				if (!isSelected(file))
					selectedNodes_.insert(file);
				else
					selectedNodes_.erase(file);
			}
			else
			{
				selectedNodes_.clear();
				selectedNodes_.insert(file);
			}
			update();

			emit fileSelected(file);
		}
	}
}

void TreemapWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		DirectoryNode *dir = detectDirectory(event->x(), event->y());

		if (dir != 0)
		{
			currentRoot_ = dir;
			update();

			emit rootChanged(currentRoot_->getName());
		}
	}
}

bool sortFunc(AbstractNode *i, AbstractNode *j)
{
	return i->getSize() > j->getSize();
}

void TreemapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
	QSize size = event->rect().size();
	QRectF rectangle;

	// if legend is shown, change filetree rectangle to smaller and
	// render legend above it
	if (showLegend_ && renderer_->hasLegend())
	{
		rectangle.setRect(0, LEGEND_HEIGHT+LEGEND_MARGIN, size.width()-1,
						  size.height()-1-LEGEND_HEIGHT-LEGEND_MARGIN);
		QRectF legendRect(0, 0, size.width()-1, LEGEND_HEIGHT);
		renderer_->renderLegend(painter, legendRect);
	}
	else
		rectangle.setRect(0, 0, size.width()-1, size.height()-1);

	// draw tree
	if (!currentRoot_->empty())
	{
		// prepare data for rendering
		DirectoryNode *dir = currentRoot_;
		list<AbstractNode *> children;
		for (int i=0; i<dir->getFileCount(); i++)
			children.push_back(dir->getFile(i));
		for (int i=0; i<dir->getDirCount(); i++)
			children.push_back(dir->getDir(i));
		children.sort(sortFunc);

		// clear detection tree
		if (detectRoot_)
			delete detectRoot_;
		detectRoot_ = new DetectionNode(QRectF(0, 0, width(), height()));

		// render black filling and files
		painter.setBrush(QColor(0, 0, 0));
		painter.drawRect(rectangle);
		drawHorz(painter, rectangle, children, detectRoot_);
	}
	// draw simple filling
	else
	{
		painter.setBrush(QColor(0xDF, 0xDF, 0xDF));
		painter.drawRect(rectangle);
	}
}

void TreemapWidget::fileTreeUpdated()
{
	currentRoot_ = tree_->getRoot();
	selectedNodes_.clear();
    update();

	emit rootChanged(currentRoot_->getName());
}

void TreemapWidget::back()
{
	if (currentRoot_->getParent()) {
		currentRoot_ = (DirectoryNode*) currentRoot_->getParent();
		update();

		emit rootChanged(currentRoot_->getName());
	}
}

float TreemapWidget::listSum(list<AbstractNode*> &l)
{
	list<AbstractNode*>::iterator it;
	float sum = 0;
	for (it = l.begin(); it != l.end(); ++it)
		sum += (*it)->getSize();
	return sum;
}

float TreemapWidget::worstVert(list<AbstractNode*> &l, double &sum, double &dirSize, QRectF &r)
{
	list<AbstractNode*>::iterator it;
	float worst = 0, tmp;

	for (it = l.begin(); it != l.end(); ++it)
	{
		tmp = (float) sum*sum*r.width()/(dirSize*(*it)->getSize()*r.height());
		if (tmp < 1) tmp = (float) 1.0 / tmp;
		worst = max<float>(worst, tmp);
	}
	return worst;
}

float TreemapWidget::worstHorz(list<AbstractNode*> &l, double &sum, double &dirSize, QRectF &r)
{
	list<AbstractNode*>::iterator it;
	float worst = 0, tmp;

	for (it = l.begin(); it != l.end(); ++it)
	{
		tmp = (float) dirSize*r.width()*(*it)->getSize()/(sum*sum*r.height());
		if (tmp < 1) tmp = (float) 1.0 / tmp;
		worst = max<float>(worst, tmp);
	}
	return worst;
}

void TreemapWidget::drawVert(QPainter &painter, QRectF &rect, list<AbstractNode*> &children, DetectionNode *node)
{
	QRectF r;
	list<AbstractNode*> row;
	double dirSize = listSum(children), sum = 0;
	float lastWorst, currWorst, wi, he;

	row.push_back( children.front() );
	sum += children.front()->getSize();
	children.pop_front();
	lastWorst = worstVert(row, sum, dirSize, rect);

	while (!children.empty())
	{
		row.push_back( children.front() );
		sum += children.front()->getSize();
		children.pop_front();
		currWorst = worstVert(row, sum, dirSize, rect);

		if (currWorst > lastWorst)
		{
			children.push_front( row.back() );
			sum -= row.back()->getSize();
			row.pop_back();
			break;
		}
		else if (children.empty())
			break;
		else
			lastWorst = currWorst;
	}

	if (!row.empty())
	{
		float offset = 0;
		for (list<AbstractNode*>::iterator it=row.begin(); it!=row.end(); ++it)
		{
			wi = (float) sum*rect.width()/dirSize;
			he = (float) (*it)->getSize()*rect.height()/sum;
			r.setRect(rect.x(), rect.y()+offset, wi, he);

			if ((*it)->isDir())
			{
				list<AbstractNode*> grandChildren;
				DirectoryNode *dir = (DirectoryNode* ) *it;

				if (dir->getDirCount()+dir->getFileCount() > 0)
				{
					for (int i=0; i<dir->getFileCount(); ++i)
						grandChildren.push_back(dir->getFile(i));
					for (int i=0; i<dir->getDirCount(); ++i)
						grandChildren.push_back(dir->getDir(i));

					DetectionNode *newNode = new DetectionNode(r);
					node->addChild(newNode);

					grandChildren.sort(sortFunc);
					if (r.height() < r.width())
						drawHorz(painter, r, grandChildren, newNode);
					else
						drawVert(painter, r, grandChildren, newNode);
				}
			}
			else
			{
				FileNode *file = (FileNode *)(*it);
				DetectionNode *newNode = new DetectionNode(r, file);
				node->addChild(newNode);

				if (r.height() > 2 && r.width() > 2)
					renderer_->renderCell(painter, r, file);
			}

			offset += he;
		}
	}

	if (!children.empty())
	{
		wi = (float) sum*rect.width()/dirSize;
		r.setRect(rect.x()+wi, rect.y(), rect.width()-wi, rect.height());

		if (r.height() < r.width())
			drawHorz(painter, r, children, node);
		else
			drawVert(painter, r, children, node);
	}
}

void TreemapWidget::drawHorz(QPainter &painter, QRectF &rect, list<AbstractNode*> &children, DetectionNode *node)
{
	QRectF r;
	list<AbstractNode*> row;
	double dirSize = listSum(children), sum = 0;
	float lastWorst, currWorst, wi, he;

	row.push_back( children.front() );
	sum += children.front()->getSize();
	children.pop_front();
	lastWorst = worstHorz(row, sum, dirSize, rect);

	while (!children.empty())
	{
		row.push_back( children.front() );
		sum += children.front()->getSize();
		children.pop_front();
		currWorst = worstHorz(row, sum, dirSize, rect);

		if (currWorst > lastWorst)
		{
			children.push_front( row.back() );
			sum -= row.back()->getSize();
			row.pop_back();
			break;
		}
		else if (children.empty())
			break;
		else
			lastWorst = currWorst;
	}

	if (!row.empty())
	{
		float offset = 0;
		for (list<AbstractNode*>::iterator it=row.begin(); it!=row.end(); ++it)
		{
			wi = (float) (*it)->getSize()*rect.width()/sum;
			he = (float) sum*rect.height()/dirSize;
			r.setRect(rect.x()+offset, rect.y(), wi, he);

			if ((*it)->isDir())
			{
				list<AbstractNode*> grandChildren;
				DirectoryNode *dir = (DirectoryNode* ) *it;

				if (dir->getDirCount()+dir->getFileCount() > 0)
				{
					for (int i=0; i<dir->getFileCount(); ++i)
						grandChildren.push_back(dir->getFile(i));
					for (int i=0; i<dir->getDirCount(); ++i)
						grandChildren.push_back(dir->getDir(i));

					DetectionNode *newNode = new DetectionNode(r);
					node->addChild(newNode);

					grandChildren.sort(sortFunc);
					if (r.height() < r.width())
						drawHorz(painter, r, grandChildren, newNode);
					else
						drawVert(painter, r, grandChildren, newNode);
				}
			}
			else
			{
				FileNode *file = (FileNode *)(*it);
				DetectionNode *newNode = new DetectionNode(r, file);
				node->addChild(newNode);

				if (r.height() > 2 && r.width() > 2)
					renderer_->renderCell(painter, r, file);
			}

			offset += wi;
		}
	}

	if (!children.empty())
	{
		he = (float) sum*rect.height()/dirSize;
		r.setRect(rect.x(), rect.y()+he, rect.width(), rect.height()-he);
		if (r.height() < r.width())
			drawHorz(painter, r, children, node);
		else
			drawVert(painter, r, children, node);
	}
}

FileNode *TreemapWidget::detectFile(int x, int y)
{
	DetectionNode *dNode = detectRoot_;

	if (currentRoot_->empty()) return 0;
	if (detectRoot_ == 0) return 0;

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

DirectoryNode *TreemapWidget::detectDirectory(int x, int y)
{
	AbstractNode *node = detectFile(x, y);

	if (node == 0) return 0;
	if (node->getParent() == currentRoot_) return currentRoot_;

	while (node->getParent() != currentRoot_)
		node = node->getParent();
	return (DirectoryNode*) node;
}
