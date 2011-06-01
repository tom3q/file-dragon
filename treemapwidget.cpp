/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	treemapwidget.cpp
*	TreemapWidget class definition.
*/

#include <QtGui>
#include <QDebug>

#include "treemapwidget.h"
#include "directorynode.h"

using namespace std;

TreemapWidget::TreemapWidget(QWidget *parent) :
#ifdef USE_OPENGL
QGLWidget(parent)
#else
QWidget(parent)
#endif
{
	setAutoFillBackground(true);

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

void TreemapWidget::setRenderer(AbstractRenderer *cr)
{
	AbstractRenderer *ptr = renderer_;
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

const set<FileNode*> TreemapWidget::getSelectedFiles() const
{
	return selectedNodes_;
}

void TreemapWidget::removeSelection()
{
	selectedNodes_.clear();
	update();
}

void TreemapWidget::setMarkedFiles(set<FileNode *> s)
{
	tree_->setMarkedFiles(s);
	update();
}

bool TreemapWidget::isSelected(FileNode *node) const
{
	return selectedNodes_.find(node) != selectedNodes_.end();
}

void TreemapWidget::setLegendVisible(bool show)
{
	showLegend_ = show;
	update();
}

bool TreemapWidget::isLegendVisible() const
{
	return showLegend_;
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
		multiset<AbstractNode *, SortFunc> children;
		for (int i=0; i<dir->getFileCount(); i++)
			children.insert(dir->getFile(i));
		for (int i=0; i<dir->getDirCount(); i++)
			children.insert(dir->getDir(i));

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

float TreemapWidget::listSum(multiset<AbstractNode *, SortFunc> &l)
{
	multiset<AbstractNode *, SortFunc>::iterator it;
	float sum = 0;
	for (it = l.begin(); it != l.end(); ++it)
		sum += (*it)->getSize();
	return sum;
}

float TreemapWidget::worstVert(multiset<AbstractNode *, SortFunc> &l, double &sum, double &dirSize, QRectF &r)
{
	float worst = 0, tmp;

	float intermediate = sum*sum*r.width() / (dirSize*r.height());
	tmp = (float) intermediate/(*l.begin())->getSize();
	if (tmp < 1)
		tmp = (float) 1.0 / tmp;
	worst = max<float>(worst, tmp);
	tmp = (float) intermediate/(*l.rbegin())->getSize();
	if (tmp < 1)
		tmp = (float) 1.0 / tmp;
	worst = max<float>(worst, tmp);

	return worst;
}

float TreemapWidget::worstHorz(multiset<AbstractNode *, SortFunc> &l, double &sum, double &dirSize, QRectF &r)
{
	float worst = 0, tmp;

	float intermediate = dirSize*r.width()/(sum*sum*r.height());
	tmp = (float) intermediate*(*l.begin())->getSize();
	if (tmp < 1)
		tmp = (float) 1.0 / tmp;
	worst = max<float>(worst, tmp);
	tmp = (float) intermediate*(*l.rbegin())->getSize();
	if (tmp < 1)
		tmp = (float) 1.0 / tmp;
	worst = max<float>(worst, tmp);

	return worst;
}

void TreemapWidget::drawVert(QPainter &painter, QRectF &rect, multiset<AbstractNode *, SortFunc> &children, DetectionNode *node)
{
	QRectF r;
	multiset<AbstractNode *, SortFunc> row;
	double dirSize = listSum(children), sum = 0;
	float lastWorst, currWorst, wi, he;

	multiset<AbstractNode *, SortFunc>::iterator first;
	first = children.begin();
	row.insert( *first );
	sum += (*first)->getSize();
	children.erase(first);
	lastWorst = worstVert(row, sum, dirSize, rect);

	while (!children.empty())
	{
		multiset<AbstractNode *, SortFunc>::iterator it;
		it = row.insert( *children.begin() );
		sum += (*children.begin())->getSize();
		children.erase(children.begin());
		currWorst = worstVert(row, sum, dirSize, rect);

		if (currWorst > lastWorst)
		{
			children.insert( *it );
			sum -= (*it)->getSize();
			row.erase(it);
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
		for (multiset<AbstractNode *, SortFunc>::iterator it=row.begin(); it!=row.end(); ++it)
		{
			wi = (float) sum*rect.width()/dirSize;
			he = (float) (*it)->getSize()*rect.height()/sum;
			r.setRect(rect.x(), rect.y()+offset, wi, he);

			if ((*it)->isDir())
			{
				multiset<AbstractNode *, SortFunc> grandChildren;
				DirectoryNode *dir = (DirectoryNode* ) *it;

				if (dir->getDirCount()+dir->getFileCount() > 0)
				{
					for (int i=0; i<dir->getFileCount(); ++i)
						grandChildren.insert(dir->getFile(i));
					for (int i=0; i<dir->getDirCount(); ++i)
						grandChildren.insert(dir->getDir(i));

					DetectionNode *newNode = new DetectionNode(r);
					node->addChild(newNode);

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

void TreemapWidget::drawHorz(QPainter &painter, QRectF &rect, multiset<AbstractNode *, SortFunc> &children, DetectionNode *node)
{
	QRectF r;
	multiset<AbstractNode *, SortFunc> row;
	double dirSize = listSum(children), sum = 0;
	float lastWorst, currWorst, wi, he;
	multiset<AbstractNode *, SortFunc>::iterator first;

	first = children.begin();
	row.insert( *first );
	sum += (*first)->getSize();
	children.erase(first);
	lastWorst = worstHorz(row, sum, dirSize, rect);

	while (!children.empty())
	{
		multiset<AbstractNode *, SortFunc>::iterator it;
		it = row.insert( *children.begin() );
		sum += (*children.begin())->getSize();
		children.erase(children.begin());
		currWorst = worstHorz(row, sum, dirSize, rect);

		if (currWorst > lastWorst)
		{
			children.insert( *it );
			sum -= (*it)->getSize();
			row.erase(it);
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
		for (multiset<AbstractNode *, SortFunc>::iterator it=row.begin(); it!=row.end(); ++it)
		{
			wi = (float) (*it)->getSize()*rect.width()/sum;
			he = (float) sum*rect.height()/dirSize;
			r.setRect(rect.x()+offset, rect.y(), wi, he);

			if ((*it)->isDir())
			{
				multiset<AbstractNode *, SortFunc> grandChildren;
				DirectoryNode *dir = (DirectoryNode* ) *it;

				if (dir->getDirCount()+dir->getFileCount() > 0)
				{
					for (int i=0; i<dir->getFileCount(); ++i)
						grandChildren.insert(dir->getFile(i));
					for (int i=0; i<dir->getDirCount(); ++i)
						grandChildren.insert(dir->getDir(i));

					DetectionNode *newNode = new DetectionNode(r);
					node->addChild(newNode);

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
	if (showLegend_ && renderer_->hasLegend() &&
		y < LEGEND_HEIGHT+LEGEND_MARGIN)
		return 0;

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
