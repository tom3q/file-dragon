#include "daterenderer.h"
#include "treemapwidget.h"

DateRenderer::DateRenderer(TreemapWidget *w) :
	DefaultRenderer(w)
{
	now_ = QDateTime::currentDateTime();
}

void DateRenderer::renderCell(QPainter &painter, QRectF &rect, FileNode *file)
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

	if (file->isMarked())
	{
		painter.setBrush(Qt::DiagCrossPattern);
		painter.drawRect(rect);
	}
}

void DateRenderer::renderLegend(QPainter &painter, QRectF &rect)
{
	// frame
	painter.setBrush(QColor(250, 250, 250));
	painter.drawRect(rect);

	// labels
	painter.drawText(QRectF(0, 0, TEXT_WIDTH, rect.height()),
		Qt::AlignRight | Qt::AlignVCenter, "Old file");
	painter.drawText(QRectF(rect.width()-TEXT_WIDTH, 0, TEXT_WIDTH, rect.height()),
		Qt::AlignLeft | Qt::AlignVCenter, "New file");

	// gradient
	QLinearGradient grad(TEXT_WIDTH+MARGIN, rect.height()/2,
		rect.width()-TEXT_WIDTH-MARGIN, rect.height()/2);
	QRectF gradRect(TEXT_WIDTH+MARGIN, MARGIN, rect.width()-2*(TEXT_WIDTH+MARGIN),
		rect.height()-2*MARGIN);
	grad.setColorAt(0, QColor(OLD_R, OLD_G, OLD_B));
	grad.setColorAt(1, QColor(NEW_R, NEW_G, NEW_B));
	painter.fillRect(gradRect, grad);

	painter.setBrush(Qt::transparent);
	painter.drawRect(gradRect);
}

bool DateRenderer::hasLegend() const
{
	return true;
}
