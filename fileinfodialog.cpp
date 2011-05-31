/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	fileinfodialog.cpp
*	FileInfoDialog class implementation.
*/

#include <QSettings>
#include "fileinfodialog.h"
#include "ui_fileinfodialog.h"

FileInfoDialog::FileInfoDialog(QWidget *parent, FileFrame *frame) :
	QDialog(parent),
	ui(new Ui::FileInfoDialog)
{
	frame_ = frame;
	ui->setupUi(this);
	loadSettings();
}

FileInfoDialog::~FileInfoDialog()
{
	saveSettings();
	delete ui;
}

void FileInfoDialog::saveSettings()
{
	QSettings settings;
	settings.setValue("fileinfodialog/name", ui->checkBox->isChecked());
	settings.setValue("fileinfodialog/size", ui->checkBox_2->isChecked());
	settings.setValue("fileinfodialog/moddate", ui->checkBox_3->isChecked());
}

void FileInfoDialog::loadSettings()
{
	QSettings settings;
	bool check1 = settings.value("fileinfodialog/name", true).toBool();
	bool check2 = settings.value("fileinfodialog/size", false).toBool();
	bool check3 = settings.value("fileinfodialog/moddate", false).toBool();
	ui->checkBox->setChecked( check1 );
	ui->checkBox_2->setChecked( check2 );
	ui->checkBox_3->setChecked( check3 );

	// apply settings to FileFrame
	on_pushButton_2_clicked();
}

void FileInfoDialog::changeEvent(QEvent *e)
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

void FileInfoDialog::on_pushButton_clicked()
{
	hide();
}

void FileInfoDialog::on_pushButton_2_clicked()
{
	int flags = 0;
	if (ui->checkBox->isChecked())
		flags |= FileFrame::NAME;
	if (ui->checkBox_2->isChecked())
		flags |= FileFrame::SIZE;
	if (ui->checkBox_3->isChecked())
		flags |= FileFrame::MOD_DATE;

	frame_->setFlags(flags);
	hide();
}
