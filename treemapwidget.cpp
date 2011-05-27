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
    setAutoFillBackground(false);

    // create file tree and connect signals-slots
    tree_ = new FileTree();
    connect(tree_, SIGNAL(treeUpdated()), this, SLOT(fileTreeUpdated()));

    // create a cell renderer
	renderer_ = new CellRenderer(this);
}

TreemapWidget::~TreemapWidget()
{
    delete tree_;
    delete renderer_;
}

void TreemapWidget::setCellRenderer(CellRenderer *cr)
{
    if (renderer_ != 0)
        delete renderer_;
    renderer_ = cr;
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
    painter.setBrush(QColor(180, 180, 160));

    // ramka
    QSize size = event->rect().size();
    painter.drawRect(0, 0, size.width()-1, size.height()-1);

    if (!tree_->isEmpty())
    {
        QRectF rect(0, 0, size.width()-1, size.height()-1);
        drawDirVert(painter, rect, tree_->getRoot());
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
			renderer_->render(painter, newRect, dir->getFile(i));
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
			renderer_->render(painter, newRect, dir->getFile(i));
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

	if (dir == 0) return 0;

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
