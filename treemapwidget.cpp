#include <QtGui>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <QDebug>

#include "treemapwidget.h"
#include "directorynode.h"

using namespace std;

TreemapWidget::TreemapWidget(QWidget *parent) :
    QWidget(parent)
{
	setAutoFillBackground(true);

    // create file tree and connect signals-slots
    tree_ = new FileTree();
	currentRoot_ = tree_->getRoot();

    // create a cell renderer
	renderer_ = new DefaultRenderer(this);

	showLegend_ = false;
}

TreemapWidget::~TreemapWidget()
{
    delete tree_;
    delete renderer_;
}

void TreemapWidget::setRenderer(DefaultRenderer *cr)
{
	DefaultRenderer *ptr = renderer_;
	renderer_ = cr;

	if (ptr != 0)
		delete ptr;

	repaint();
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

	for (it = selectedNodes_.cbegin(); it != selectedNodes_.cend(); ++it)
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
	repaint();
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
			repaint();

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
			startVert ^= 1;
			currentRoot_ = dir;
			repaint();

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

	// black filling
	painter.setBrush(QColor(0, 0, 0));
	painter.drawRect(rectangle);

	// draw tree
    if (!currentRoot_->empty()) {
		if (startVert)
			drawDirVert(painter, rectangle, currentRoot_);
		else
			drawDirHorz(painter, rectangle, currentRoot_);
	// draw simple filling
	}
	else
	{
		painter.setBrush(QColor(0xDF, 0xDF, 0xDF));
		painter.drawRect(rectangle);
	}
}

void TreemapWidget::fileTreeUpdated()
{
	startVert = true;
	currentRoot_ = tree_->getRoot();
	selectedNodes_.clear();
    update();

	emit rootChanged(currentRoot_->getName());
}

void TreemapWidget::back()
{
	if (currentRoot_->getParent()) {
		currentRoot_ = (DirectoryNode*) currentRoot_->getParent();
		startVert ^= 1;
		update();

		emit rootChanged(currentRoot_->getName());
	}
}

void TreemapWidget::drawDirVert(QPainter &painter, QRectF &rect, DirectoryNode *dir)
{
    QRectF newRect;
    double dirSize = dir->getSize();
    float offset = 0, rectWidth;

    for (int i=0; i<dir->getFileCount(); i++)
    {
        rectWidth = (double) rect.width() * dir->getFile(i)->getSize() / dirSize;
        newRect.setRect(rect.x()+offset, rect.y(), rectWidth, rect.height());

		if (newRect.width() >= 1 && newRect.height() >= 1)
			renderer_->renderCell(painter, newRect, dir->getFile(i));
        offset += rectWidth;
    }

    for (int i=0; i<dir->getDirCount(); i++)
    {
        if (dir->getDir(i)->getSize() == 0) continue;

        rectWidth = (double) rect.width() * dir->getDir(i)->getSize() / dirSize;
        newRect.setRect(rect.x()+offset, rect.y(), rectWidth, rect.height());

        drawDirHorz(painter, newRect, dir->getDir(i));
        offset += rectWidth;
    }
}

void TreemapWidget::drawDirHorz(QPainter &painter, QRectF &rect, DirectoryNode *dir)
{
    QRectF newRect;
    double dirSize = dir->getSize();
    float offset = 0, rectHeight;

    for (int i=0; i<dir->getFileCount(); i++)
    {
        rectHeight = (double) rect.height() * dir->getFile(i)->getSize() / dirSize;
        newRect.setRect(rect.x(), rect.y()+offset, rect.width(), rectHeight);

		if (newRect.height() >= 1 && newRect.width() >= 1)
			renderer_->renderCell(painter, newRect, dir->getFile(i));
        offset += rectHeight;
    }

    for (int i=0; i<dir->getDirCount(); i++)
    {
        if (dir->getDir(i)->getSize() == 0) continue;

        rectHeight = (double) rect.height() * dir->getDir(i)->getSize() / dirSize;
        newRect.setRect(rect.x(), rect.y()+offset, rect.width(), rectHeight);

        drawDirVert(painter, newRect, dir->getDir(i));
        offset += rectHeight;
    }
}

DirectoryNode *TreemapWidget::detectDirectory(int x, int y)
{
	DirectoryNode *dir = currentRoot_;
	QRectF dirRect = rect(), currRect;
	double dirSize, wi, he;
	int vert = startVert;

	if (currentRoot_->empty()) return 0;

	// if legend is shown, drawing is moved down and detection rectangle
	// also should be moved down
	if (showLegend_ && renderer_->hasLegend())
	{
		dirRect.setY(LEGEND_HEIGHT+LEGEND_MARGIN);

		// if users clicked legend, no detection should take place
		if (y <= LEGEND_HEIGHT+LEGEND_MARGIN)
			return 0;
	}

	dirSize = dir->getSize();

	if (vert % 2)
	{
		currRect.setX(dirRect.x());
		currRect.setY(dirRect.y());
		currRect.setHeight(dirRect.height());

		for (int i = 0; i < dir->getFileCount(); i++)
		{
			wi = (double) dirRect.width() * dir->getFile(i)->getSize() / dirSize;
			currRect.setWidth(wi);

			if (currRect.contains(x, y))
				return 0;

			currRect.setX(currRect.x() + currRect.width());
		}

		for (int i = 0; i < dir->getDirCount(); i++)
		{
			wi = (double) dirRect.width() * dir->getDir(i)->getSize() / dirSize;
			currRect.setWidth(wi);

			if (currRect.contains(x, y))
				return dir->getDir(i);

			currRect.setX(currRect.x() + currRect.width());
		}
	}
	else
	{
		currRect.setX(dirRect.x());
		currRect.setY(dirRect.y());
		currRect.setWidth(dirRect.width());

		for (int i = 0; i < dir->getFileCount(); i++)
		{
			he = (double) dirRect.height() * dir->getFile(i)->getSize() / dirSize;
			currRect.setHeight(he);

			if (currRect.contains(x, y))
				return 0;

			currRect.setY(currRect.y() + currRect.height());
		}

		for (int i = 0; i < dir->getDirCount(); i++)
		{
			he = (double) dirRect.height() * dir->getDir(i)->getSize() / dirSize;
			currRect.setHeight(he);

			if (currRect.contains(x, y))
				return dir->getDir(i);

			currRect.setY(currRect.y() + currRect.height());
		}
	}

	return 0;
}

FileNode *TreemapWidget::detectFile(int x, int y)
{
	DirectoryNode *dir = currentRoot_;
	QRectF dirRect = rect(), currRect;
	double dirSize, wi, he;
	int vert = startVert;

	if (currentRoot_->empty()) return 0;

	// if legend is shown, drawing is moved down and detection rectangle
	// also should be moved down
	if (showLegend_ && renderer_->hasLegend())
	{
		dirRect.setY(LEGEND_HEIGHT+LEGEND_MARGIN);

		// if users clicked legend, no detection should take place
		if (y <= LEGEND_HEIGHT+LEGEND_MARGIN)
			return 0;
	}

	while (true)
	{
		dirSize = dir->getSize();

		if (vert % 2)
		{
			currRect.setX(dirRect.x());
			currRect.setY(dirRect.y());
			currRect.setHeight(dirRect.height());

			for (int i = 0; i < dir->getFileCount(); i++)
			{
				wi = (double) dirRect.width() * dir->getFile(i)->getSize() / dirSize;
				currRect.setWidth(wi);

				if (currRect.contains(x, y))
					return dir->getFile(i);

				currRect.setX(currRect.x() + currRect.width());
			}

			for (int i = 0; i < dir->getDirCount(); i++)
			{
				wi = (double) dirRect.width() * dir->getDir(i)->getSize() / dirSize;
				currRect.setWidth(wi);

				if (currRect.contains(x, y))
				{
					dir = dir->getDir(i);
					dirRect = currRect;
					vert++;
					break;
				}

				currRect.setX(currRect.x() + currRect.width());
			}
		}
		else
		{
			currRect.setX(dirRect.x());
			currRect.setY(dirRect.y());
			currRect.setWidth(dirRect.width());

			for (int i = 0; i < dir->getFileCount(); i++)
			{
				he = (double) dirRect.height() * dir->getFile(i)->getSize() / dirSize;
				currRect.setHeight(he);

				if (currRect.contains(x, y))
					return dir->getFile(i);

				currRect.setY(currRect.y() + currRect.height());
			}

			for (int i = 0; i < dir->getDirCount(); i++)
			{
				he = (double) dirRect.height() * dir->getDir(i)->getSize() / dirSize;
				currRect.setHeight(he);

				if (currRect.contains(x, y))
				{
					dir = dir->getDir(i);
					dirRect = currRect;
					vert++;
					break;
				}

				currRect.setY(currRect.y() + currRect.height());
			}
		}
	}
}
