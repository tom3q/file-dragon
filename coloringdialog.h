#ifndef COLORINGDIALOG_H
#define COLORINGDIALOG_H

#include <QDialog>

#include "treemapwidget.h"

namespace Ui {
    class ColoringDialog;
}

class ColoringDialog : public QDialog
{
    Q_OBJECT

public:
	explicit ColoringDialog(QWidget *parent, TreemapWidget *cd);
    ~ColoringDialog();

protected:
    void changeEvent(QEvent *e);

private slots:
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();

private:
    Ui::ColoringDialog *ui;
	TreemapWidget *treeWidget_;
};

#endif // COLORINGDIALOG_H
