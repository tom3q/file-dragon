#ifndef FILEINFODIALOG_H
#define FILEINFODIALOG_H

#include <QDialog>

#include "fileframe.h"

namespace Ui {
	class FileInfoDialog;
}

class FileInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FileInfoDialog(QWidget *parent, FileFrame *frame);
	~FileInfoDialog();

	void saveSettings();
	void loadSettings();

protected:
	void changeEvent(QEvent *e);

private slots:
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();

private:
	Ui::FileInfoDialog *ui;
	FileFrame *frame_;
};

#endif // FILEINFODIALOG_H
