#include "datecellrenderer.h"
#include "treemapwidget.h"

DateCellRenderer::DateCellRenderer(TreemapWidget *w) :
	CellRenderer(w)
{
	now_ = QDateTime::currentDateTime();
}

void DateCellRenderer::render(QPainter &painter, QRectF &rect, FileNode *file)
{
	int days = file->getModified().daysTo(now_);
	float ratio = (float) days / (5*365);
	if (ratio > 1) ratio = 1;

	int r = (OLD_R-NEW_R) * ratio + NEW_R;
	int g = (OLD_G-NEW_G) * ratio + NEW_G;
	int b = (OLD_B-NEW_B) * ratio + NEW_B;

	if (widget_->isSelected(file))
		painter.setBrush(QColor(r+HIGHLIGHT, g+HIGHLIGHT, b+HIGHLIGHT));
	else
		painter.setBrush(QColor(r, g, b));

	painter.drawRect(rect);
}
