#include <QtGui>

#include "treemapwidget.h"

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
}

/**
  * Destructor.
  */
TreemapWidget::~TreemapWidget()
{
    delete tree_;
}

/**
  * Returns a reference to its FileTree object.
  * @return A Filetree reference.
  */
FileTree &TreemapWidget::getFileTree()
{
    return *tree_;
}

/**
  * In this event all painting should take place.
  */
void TreemapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QColor(200, 100, 100));

    // ramka
    QSize size = event->rect().size();
    painter.drawRect(0, 0, size.width()-1, size.height()-1);
}

/**
  * This slot is called when FileTree changes its contents.
  */
void TreemapWidget::fileTreeUpdated()
{
    update();
}
