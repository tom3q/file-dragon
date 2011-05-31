/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	daterenderer.h
*	DateRenderer class prototype.
*/

#ifndef DATECELLRENDERER_H
#define DATECELLRENDERER_H

#include "defaultrenderer.h"

class TreemapWidget;

/**
 * DateRenderer extends DefaultRenderer with rendering
 * of coloured rectangles based on file modification date.
 * It also includes a legend.
 */
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

	/**
	 * Object constructor. Gets a pointer to TreemapWidget so
	 * that the rendering method could determine if a file
	 * rectangle is selected or not.
	 * @param w A pointer to a TreemapWidget
	 */
	DateRenderer(TreemapWidget *w);

	/**
	 * Rendering method.
	 * @param painter TreemapWidgets QPainter object
	 * @param rect A rectangle where a file is drawn
	 * @param file A pointer to FileNode structure
	 */
	virtual void renderCell(QPainter &painter, QRectF &rect, FileNode *file);

	/**
	 * Renders a legend.
	 * @param painter TreemapWidgets QPainter object
	 * @param rect A rectangle where a legend is drawn
	 */
	virtual void renderLegend(QPainter &painter, QRectF &rect);

	/**
	 * Specifies if a renderer has a legend to draw.
	 * @return True, if it has. False if not.
	 */
	virtual inline bool hasLegend() const
	{
		return true;
	}

private:
	QDateTime now_;
};

#endif // DATECELLRENDERER_H
