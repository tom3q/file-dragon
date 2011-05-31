#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include "filetree.h"

namespace Ui {
	class FilterDialog;
}

class FilterDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FilterDialog(QWidget *parent, FileTree *tree);
	~FilterDialog();

	void saveSettings();
	void loadSettings();

protected:
	void changeEvent(QEvent *e);

private slots:
	void on_checkBox_stateChanged(int );
	void on_checkBox_2_stateChanged(int );
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();

private:
	Ui::FilterDialog *ui;
	FileTree *tree_;
};

#endif // FILTERDIALOG_H
