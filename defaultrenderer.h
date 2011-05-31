/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	defaultrenderer.h
*	DefaultRenderer class prototype.
*/

#ifndef DEFAULTRENDERER_H
#define DEFAULTRENDERER_H

#include <QtGui>

#include "abstractrenderer.h"
#include "filenode.h"

class TreemapWidget;

/**
 * DefaultRenderer implements an AbstractRenderer which renders
 * identical rectangles regardless of the given FileNode.
 */
class DefaultRenderer : public AbstractRenderer
{
public:
	const static int COLOR_R = 0xDF;
	const static int COLOR_G = 0xDF;
	const static int COLOR_B = 0xDF;
	const static int HIGHLIGHT = 20;

	/**
	 * Object constructor. Gets a pointer to TreemapWidget so
	 * that the rendering method could determine if a file
	 * rectangle is selected or not.
	 * @param w A pointer to a TreemapWidget
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
	 * Renders a legend. (Unused)
	 */
	virtual inline void renderLegend(QPainter &, QRectF &) {};

	/**
	 * Specifies if a renderer has a legend to draw.
	 * @return True, if it has. False if not.
	 */
	virtual inline bool hasLegend() const
	{
		return false;
	}

protected:
	TreemapWidget *widget_;
};

#endif // DEFAULTRENDERER_H
