#include "coloringdialog.h"
#include "ui_coloringdialog.h"
#include "defaultrenderer.h"
#include "daterenderer.h"
#include <cassert>

ColoringDialog::ColoringDialog(QWidget *parent, TreemapWidget *cd) :
	QDialog(parent),
	ui(new Ui::ColoringDialog)
{
	treeWidget_ = cd;
	ui->setupUi(this);
	loadSettings();
}

ColoringDialog::~ColoringDialog()
{
	saveSettings();
	delete ui;
}

void ColoringDialog::saveSettings()
{
	QSettings settings;
	int option;
	if (ui->radioButton->isChecked())
		option = 0;
	else
		option = 1;
	settings.setValue("coloringdialog/option", option);
}

void ColoringDialog::loadSettings()
{
	QSettings settings;
	int option = settings.value("coloringdialog/option", 0).toInt();
	if (option == 0)
		ui->radioButton->setChecked(true);
	else
		ui->radioButton_2->setChecked(true);

	// apply rendering method
	on_pushButton_2_clicked();
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
	DefaultRenderer *renderer = 0;

	if (ui->radioButton->isChecked())
		renderer = new DefaultRenderer(treeWidget_);
	else if (ui->radioButton_2->isChecked())
		renderer = new DateRenderer(treeWidget_);

	assert(renderer != 0);

	hide();
	treeWidget_->setRenderer(renderer);
}
