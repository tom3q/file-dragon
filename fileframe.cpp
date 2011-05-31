#include <QSettings>
#include <cstdio>
#include "fileframe.h"

FileFrame::FileFrame(QWidget *parent) :
	QFrame(parent)
{
	setFrameShape(QFrame::Box);
	setFrameShadow(QFrame::Raised);

	lblName_ = new QLabel();
	lblSize_ = new QLabel();
	lblModDate_ = new QLabel();

	layout_ = new QGridLayout();
	layout_->setMargin(5);
	layout_->addWidget(lblName_, 0, 0, 1, 2);
	layout_->addWidget(lblSize_, 1, 0);
	layout_->addWidget(lblModDate_, 1, 1);
	setLayout(layout_);

	flags_ = 0;
	setFlags(NAME);
	updateInfo(0);
}

FileFrame::~FileFrame()
{
	delete lblName_;
	delete lblSize_;
	delete lblModDate_;
	delete layout_;
}

void FileFrame::updateInfo(FileNode *file)
{
	if (file == 0)
	{
		lblName_->setText(tr("File name:"));
		lblSize_->setText(tr("File size:"));
		lblModDate_->setText(tr("Modification date: "));
	}
	else
	{
		// file date
		QString date = file->getModified().toString("dd/MM/yyyy hh:mm:ss");

		// file size
		QString size;
		char buffer[64];
		if (file->getSize() > 1024*1024*1024)
		{
			sprintf(buffer, "%.2lf", (double) file->getSize()/(1024*1024*1024));
			size = QString(buffer) + " GB";
		}
		else if (file->getSize() > 1024*1024)
		{
			sprintf(buffer, "%.2lf", (double) file->getSize()/(1024*1024));
			size = QString(buffer) + " MB";
		}
		else if (file->getSize() > 1024)
		{
			sprintf(buffer, "%.2lf", (double) file->getSize()/(1024));
			size = QString(buffer) + " KB";
		}
		else
			size = QString::number(file->getSize()) + " B";

		lblName_->setText(tr("File name: ")+file->getName());
		lblSize_->setText(tr("File size: ")+size);
		lblModDate_->setText(tr("Modification date: ")+date);
	}
}

void FileFrame::setFlags(int flags)
{
	flags_ = flags;

	lblName_->hide();
	lblSize_->hide();
	lblModDate_->hide();
	layout_->removeWidget(lblName_);
	layout_->removeWidget(lblSize_);
	layout_->removeWidget(lblModDate_);

	int row = 0, col = 0;

	if (flags & NAME)
	{
		layout_->addWidget(lblName_, row++, 0, 1, 2);
		lblName_->show();
	}
	if (flags & SIZE)
	{
		layout_->addWidget(lblSize_, row, col++);
		lblSize_->show();
	}
	if (flags & MOD_DATE)
	{
		layout_->addWidget(lblModDate_, row, col);
		lblModDate_->show();
	}
}

int FileFrame::getFlags() const
{
	return flags_;
}
