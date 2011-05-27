/**
  * CellRenderer is used to render a cell in a TreemapWidget. During the
    paintEvent method, CellRenderer::render is called for each of the
    files in a treemap. It is possible to set a custom renderer for
    TreemapWidget (call setCellRenderer) and provide another method of
    drawing cells.
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

    painter.drawRect(rect);
}

void DefaultRenderer::renderLegend(QPainter &painter, QRectF &rect)
{
}

bool DefaultRenderer::hasLegend() const
{
	return false;
}
