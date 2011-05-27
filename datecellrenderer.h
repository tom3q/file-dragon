#ifndef DATECELLRENDERER_H
#define DATECELLRENDERER_H

#include "cellrenderer.h"

class TreemapWidget;

class DateCellRenderer : public CellRenderer
{
public:
	DateCellRenderer(TreemapWidget *w);

	virtual void render(QPainter &painter, QRectF &rect, FileNode *file);
};

#endif // DATECELLRENDERER_H
