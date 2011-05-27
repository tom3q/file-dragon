#ifndef DATECELLRENDERER_H
#define DATECELLRENDERER_H

#include "cellrenderer.h"

class TreemapWidget;

class DateCellRenderer : public CellRenderer
{
public:
	static const int NEW_R = 0xCA;
	static const int NEW_G = 0xE8;
	static const int NEW_B = 0xC5;

	static const int OLD_R = 0xE8;
	static const int OLD_G = 0xCE;
	static const int OLD_B = 0xC5;

	DateCellRenderer(TreemapWidget *w);

	virtual void render(QPainter &painter, QRectF &rect, FileNode *file);

private:
	QDateTime now_;
};

#endif // DATECELLRENDERER_H
