#ifndef QUEUEDIALOG_H
#define QUEUEDIALOG_H

#include <QDialog>
#include "filemanager.h"
#include <QStringListModel>

namespace Ui {
	class QueueDialog;
}

class QueueDialog : public QDialog
{
	Q_OBJECT

public:
	explicit QueueDialog(QWidget *parent, FileManager *fileManager);
	~QueueDialog();

protected:
	void changeEvent(QEvent *e);
	void showEvent(QShowEvent *e);

private slots:
	void accept();
	void reject();
	void on_clearButton_clicked();

private:
	Ui::QueueDialog *ui;
	QStringListModel *listModel_;
	FileManager *fileManager_;
};

#endif // QUEUEDIALOG_H
