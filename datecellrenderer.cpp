#include "datecellrenderer.h"
#include "cellrenderer.h"

DateCellRenderer::DateCellRenderer(TreemapWidget *w) :
	CellRenderer(w)
{
}

void DateCellRenderer::render(QPainter &painter, QRectF &rect, FileNode *file)
{
	int colorNew[] = {47, 203, 39};
	int colorOld[] = {24, 93, 26};
	QDateTime now = QDateTime::currentDateTime();
	int days = file->getModified().daysTo(now);
	float ratio = (float) days / (5*365);
	if (ratio > 1) ratio = 1;

	int r = (colorOld[0]-colorNew[0]) * ratio + colorNew[0];
	int g = (colorOld[1]-colorNew[1]) * ratio + colorNew[1];
	int b = (colorOld[2]-colorNew[2]) * ratio + colorNew[2];

	painter.setBrush(QColor(r, g, b));
	painter.drawRect(rect);
}
