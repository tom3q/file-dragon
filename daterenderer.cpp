/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	daterenderer.cpp
*	DateRenderer class implementation.
*/

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

	painter.setBrush(QColor(r, g, b));
	painter.drawRect(rect);

	QLinearGradient grad(rect.x()+rect.width()/2, rect.y(),
						 rect.x()+rect.width()/2, rect.y()+rect.height());
	QRectF gradRect(rect.x()+1, rect.y()+1, rect.width()-2, rect.height()-2);

	if (widget_->isSelected(file))
	{
		grad.setColorAt(0, QColor(255, 255, 255, 0));
		grad.setColorAt(1, QColor(255, 255, 255, 255));
		painter.fillRect(gradRect, grad);
	}

	grad.setColorAt(0.0, QColor(0, 0, 0, 30));
	grad.setColorAt(0.5, QColor(0, 0, 0, 80));
	grad.setColorAt(1.0, QColor(0, 0, 0, 30));
	painter.fillRect(gradRect, grad);

	if (file->isMarked())
	{
		painter.setBrush(QColor(0, 0, 0, 50));
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

	// color gradient
	QLinearGradient grad(TEXT_WIDTH+MARGIN, rect.height()/2,
		rect.width()-TEXT_WIDTH-MARGIN, rect.height()/2);
	QRectF gradRect(TEXT_WIDTH+MARGIN, MARGIN, rect.width()-2*(TEXT_WIDTH+MARGIN),
		rect.height()-2*MARGIN);
	grad.setColorAt(0, QColor(OLD_R, OLD_G, OLD_B));
	grad.setColorAt(1, QColor(NEW_R, NEW_G, NEW_B));
	painter.fillRect(gradRect, grad);

	// shading gradient
	QLinearGradient shade(rect.width()/2, MARGIN, rect.width()/2, rect.height()-MARGIN);
	shade.setColorAt(0.0, QColor(0, 0, 0, 30));
	shade.setColorAt(0.5, QColor(0, 0, 0, 80));
	shade.setColorAt(1.0, QColor(0, 0, 0, 30));
	painter.fillRect(gradRect, shade);

	painter.setBrush(Qt::transparent);
	painter.drawRect(gradRect);
}
