/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	abstractrenderer.h
*	AbstractRenderer class prototype.
*/

#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include <QtGui>

#include "filenode.h"

/**
 * AbstractRenderer is used to render a cell in a TreemapWidget
 * with characteristics based on the given FileNode object. During the
 * paintEvent method, CellRenderer::render is called for each of the
 * files in a treemap. It is possible to set a custom renderer for
 * TreemapWidget (call setCellRenderer) and provide another method of
 * drawing cells.
 */
class AbstractRenderer
{
public:
	/**
	 * Rendering method.
	 * @param painter TreemapWidgets QPainter object
	 * @param rect A rectangle where a file is drawn
	 * @param file A pointer to FileNode structure
	 */
	virtual void renderCell(QPainter &painter, QRectF &rect, FileNode *file) = 0;

	/**
	 * Renders a legend.
	 * @param painter TreemapWidgets QPainter object
	 * @param rect A rectangle where a legend is drawn
	 */
	virtual void renderLegend(QPainter &painter, QRectF &rect) = 0;

	/**
	 * Specifies if a renderer has a legend to draw.
	 * @return True, if it has. False if not.
	 */
	virtual bool hasLegend() const = 0;
};

#endif // ABSTRACTRENDERER_H
