#include <QtGui>
#include <iostream>
#include <cmath>

#include "treemapwidget.h"
#include "directorynode.h"

using namespace std;

/**
  * Widget constructor.
  */
TreemapWidget::TreemapWidget(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(false);

    // create file tree and connect signals-slots
    tree_ = new FileTree();
    connect(tree_, SIGNAL(treeUpdated()), this, SLOT(fileTreeUpdated()));

    // create a cell renderer
    renderer_ = new CellRenderer();
}

/**
  * Destructor.
  */
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

/**
  * Returns a reference to its FileTree object.
  * @return A Filetree reference.
  */
FileTree &TreemapWidget::getFileTree() const
{
    return *tree_;
}

/**
  * In this event all painting should take place.
  */
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

/**
  * This slot is called when FileTree changes its contents.
  */
void TreemapWidget::fileTreeUpdated()
{
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
