#include "queuedialog.h"
#include "ui_queuedialog.h"

QueueDialog::QueueDialog(QWidget *parent, FileManager *fileManager) :
	QDialog(parent),
	ui(new Ui::QueueDialog),
	fileManager_(fileManager)
{
	ui->setupUi(this);
	listModel_ = new QStringListModel();
	ui->fileList->setModel(listModel_);
}

QueueDialog::~QueueDialog()
{
	delete ui;
	delete listModel_;
}

void QueueDialog::changeEvent(QEvent *e)
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

void QueueDialog::showEvent(QShowEvent *e)
{
	listModel_->setStringList(fileManager_->getQueuedFiles());

	QDialog::showEvent(e);
}

void QueueDialog::on_clearButton_clicked()
{
	hide();
	fileManager_->clear();
}

void QueueDialog::accept()
{
	hide();
	emit accepted();
}

void QueueDialog::reject()
{
	hide();
}
