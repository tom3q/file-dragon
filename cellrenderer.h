#ifndef CELLRENDERER_H
#define CELLRENDERER_H

#include <QtGui>

#include "filenode.h"

class TreemapWidget;

class CellRenderer
{
public:
	/**
	  * Object constructor. Gets a pointer to TreemapWidget so
		that the rendering method could determine if a file
		rectangle is selected or not.
	  *	@param w A pointer to a TreemapWidget
	  */
	CellRenderer(TreemapWidget *w);

	/**
	  * Rendering method.
	  * @param painter TreemapWidgets QPainter object
	  * @param rect A rectangle where a file is drawn
	  * @param file A pointer to FileNode structure
	  */
	virtual void render(QPainter &painter, QRectF &rect, FileNode *file);

private:
	TreemapWidget *widget_;
};

#endif // CELLRENDERER_H
