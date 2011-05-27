#include <QtGui>
#include <iostream>
#include <cstdio>
#include <cmath>

#include "treemapwidget.h"
#include "directorynode.h"

using namespace std;

TreemapWidget::TreemapWidget(QWidget *parent) :
    QWidget(parent)
{
	setAutoFillBackground(true);

    // create file tree and connect signals-slots
    tree_ = new FileTree();

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

bool TreemapWidget::isSelected(FileNode *node) const
{
	return selectedNodes_.find(node) != selectedNodes_.end();
}

void TreemapWidget::setShowLegend(bool show)
{
	showLegend_ = show;
	repaint();
}

void TreemapWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		FileNode *file = detectFile(event->x(), event->y());

		if (file != 0)
		{
			// if Control key is not pressed, clear the selection.
			// if it is pressed, the selection will remain
			if (!(QApplication::keyboardModifiers() & Qt::ControlModifier))
				selectedNodes_.clear();
			selectedNodes_.insert(file);
			repaint();
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
    if (!tree_->isEmpty())
		drawDirVert(painter, rectangle, tree_->getRoot());
	// draw simple filling
	else
	{
		painter.setBrush(QColor(0xDF, 0xDF, 0xDF));
		painter.drawRect(rectangle);
	}
}

void TreemapWidget::fileTreeUpdated()
{
	selectedNodes_.clear();
    update();
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

FileNode *TreemapWidget::detectFile(int x, int y)
{
	DirectoryNode *dir = getFileTree().getRoot();
	QRectF dirRect = rect(), currRect;
	double dirSize, wi, he;
	int vert = 1;

	if (getFileTree().isEmpty()) return 0;

	// if legend is shown, drawing is moved down and detection rectangle
	// also should be moved down
	if (showLegend_ && renderer_->hasLegend())
		dirRect.setY(LEGEND_HEIGHT+LEGEND_MARGIN);

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
