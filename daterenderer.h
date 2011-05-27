#ifndef DATECELLRENDERER_H
#define DATECELLRENDERER_H

#include "defaultrenderer.h"

class TreemapWidget;

class DateRenderer : public DefaultRenderer
{
public:
	static const int NEW_R = 0xCA;
	static const int NEW_G = 0xE8;
	static const int NEW_B = 0xC5;

	static const int OLD_R = 0xE8;
	static const int OLD_G = 0xCE;
	static const int OLD_B = 0xC5;

	static const int TEXT_WIDTH = 50;
	static const int MARGIN = 5;

	DateRenderer(TreemapWidget *w);

	virtual void renderCell(QPainter &painter, QRectF &rect, FileNode *file);
	virtual void renderLegend(QPainter &painter, QRectF &rect);
	virtual bool hasLegend() const;

private:
	QDateTime now_;
};

#endif // DATECELLRENDERER_H
