#include "coloringdialog.h"
#include "ui_coloringdialog.h"
#include "cellrenderer.h"
#include "datecellrenderer.h"

ColoringDialog::ColoringDialog(QWidget *parent, TreemapWidget *cd) :
    QDialog(parent),
    ui(new Ui::ColoringDialog)
{
	treeWidget_ = cd;
    ui->setupUi(this);
}

ColoringDialog::~ColoringDialog()
{
    delete ui;
}

void ColoringDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ColoringDialog::on_pushButton_clicked()
{
	hide();
}

void ColoringDialog::on_pushButton_2_clicked()
{
	CellRenderer *renderer;

	if (ui->radioButton->isChecked())
		renderer = new CellRenderer(treeWidget_);
	else if (ui->radioButton_2->isChecked())
		renderer = new DateCellRenderer(treeWidget_);

	treeWidget_->setCellRenderer(renderer);
	hide();
}
