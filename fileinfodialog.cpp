#include "fileinfodialog.h"
#include "ui_fileinfodialog.h"

FileInfoDialog::FileInfoDialog(QWidget *parent, FileFrame *frame) :
    QDialog(parent),
    ui(new Ui::FileInfoDialog)
{
	frame_ = frame;
    ui->setupUi(this);
}

FileInfoDialog::~FileInfoDialog()
{
    delete ui;
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
