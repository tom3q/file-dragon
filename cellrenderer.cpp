/**
  * CellRenderer is used to render a cell in a TreemapWidget. During the
    paintEvent method, CellRenderer::render is called for each of the
    files in a treemap. It is possible to set a custom renderer for
    TreemapWidget (call setCellRenderer) and provide another method of
    drawing cells.
  */

#include "cellrenderer.h"

CellRenderer::CellRenderer()
{
}

/**
  * Rendering method.
  * @param painter TreemapWidgets QPainter object
  * @param rect A rectangle where a file is drawn
  * @param file A pointer to FileNode structure
  */
void CellRenderer::render(QPainter &painter, QRectF rect, FileNode *file)
{
    painter.setBrush(QColor(220, 220, 200));
    painter.drawRect(rect);
}
