/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	defaultrenderer.cpp
*	DefaultRenderer class implementation.
*/

#include "defaultrenderer.h"
#include "treemapwidget.h"

DefaultRenderer::DefaultRenderer(TreemapWidget *w)
{
	widget_ = w;
}

void DefaultRenderer::renderCell(QPainter &painter, QRectF &rect, FileNode *file)
{
	if (widget_->isSelected(file))
		painter.setBrush(QColor(COLOR_R+HIGHLIGHT, COLOR_G+HIGHLIGHT,
		COLOR_B+HIGHLIGHT));
	else
		painter.setBrush(QColor(COLOR_R, COLOR_G, COLOR_B));

	if (file->isMarked())
		painter.setBrush(QColor(0, 0, 0));

	painter.drawRect(rect);

	if (file->isMarked())
	{
		painter.setBrush(Qt::DiagCrossPattern);
		painter.drawRect(rect);
	}
}
