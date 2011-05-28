#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "filterdialog.h"
#include "coloringdialog.h"
#include "fileinfodialog.h"
#include "treemapwidget.h"
#include "filetree.h"
#include "treemanager.h"
#include "fileframe.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
	void buildTree();
	void refreshTreemap();

protected:
    void changeEvent(QEvent *e);

private slots:
	void on_actionFilters_triggered();
	void on_actionCell_coloring_triggered();
    void scanClicked();
	void scanDone();
	void cancelClicked();
	void fileClicked(FileNode *);
	void on_actionShow_legend_changed();
	void on_actionFile_information_triggered();

private:
    void createActions();
    void createMenus();
    void fillComboPartition();

	Ui::MainWindow *ui;
	FilterDialog *filterDialog;
	ColoringDialog *coloringDialog;
	FileInfoDialog *fileinfoDialog;
    TreemapWidget *treemap;
    QComboBox *comboPartition;
    QLabel *stretchWidget;
    QAction *actScan;
    QAction *actUndo;
    QAction *actRedo;
    QAction *actApply;
	QAction *actCancel;
	QAction *actBack;
	QProgressBar *scanProgress;
	FileFrame *fileFrame;

	TreeManager *treeManager;
	FileTree *tree;
};

#endif // MAINWINDOW_H
