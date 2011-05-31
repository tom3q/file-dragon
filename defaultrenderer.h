#ifndef CELLRENDERER_H
#define CELLRENDERER_H

#include <QtGui>

#include "filenode.h"

class TreemapWidget;

class DefaultRenderer
{
public:
	const static int COLOR_R = 0xDF;
	const static int COLOR_G = 0xDF;
	const static int COLOR_B = 0xDF;
	const static int HIGHLIGHT = 20;

	/**
	* Object constructor. Gets a pointer to TreemapWidget so
	that the rendering method could determine if a file
	rectangle is selected or not.
	*	@param w A pointer to a TreemapWidget
	*/
	DefaultRenderer(TreemapWidget *w);

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
	virtual bool hasLegend() const;

protected:
	TreemapWidget *widget_;
};

#endif // CELLRENDERER_H
