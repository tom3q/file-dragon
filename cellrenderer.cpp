/**
  * CellRenderer is used to render a cell in a TreemapWidget. During the
    paintEvent method, CellRenderer::render is called for each of the
    files in a treemap. It is possible to set a custom renderer for
    TreemapWidget (call setCellRenderer) and provide another method of
    drawing cells.
  */

#include "cellrenderer.h"
#include "treemapwidget.h"

CellRenderer::CellRenderer(TreemapWidget *w)
{
	widget_ = w;
}

void CellRenderer::render(QPainter &painter, QRectF &rect, FileNode *file)
{
	if (widget_->isSelected(file))
		painter.setBrush(QColor(250, 250, 230));
	else
		painter.setBrush(QColor(220, 220, 200));

    painter.drawRect(rect);
}
