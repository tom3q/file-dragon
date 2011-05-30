#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>

#include "filterdialog.h"
#include "coloringdialog.h"
#include "fileinfodialog.h"
#include "treemapwidget.h"
#include "filetree.h"
#include "treemanager.h"
#include "fileframe.h"
#include "filemanager.h"
#include "queuedialog.h"

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
	void flushQueue();

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
	void applyClicked();
	void deleteClicked();
	void queueAccepted();
	void flushDone();

private:
    void createActions();
    void createMenus();
    void fillComboPartition();

	Ui::MainWindow *ui;
	FilterDialog *filterDialog;
	ColoringDialog *coloringDialog;
	FileInfoDialog *fileinfoDialog;
	QueueDialog *queueDialog;
    TreemapWidget *treemap;
    QComboBox *comboPartition;
    QLabel *stretchWidget;
    QAction *actScan;
    QAction *actUndo;
    QAction *actRedo;
    QAction *actApply;
	QAction *actCancel;
	QAction *actBack;
	QAction *actDelete;
	QProgressBar *scanProgress;
	FileFrame *fileFrame;
	QLabel *rootPathLabel;
	QLabel *nowScanningLabel;
	QVBoxLayout *progressBarLayout;
	QWidget *progressBar;
	QMovie *progressAnim;
	QLabel *progressAnimLabel;

	TreeManager *treeManager;
	FileTree *tree;
	FileManager *fileManager;
};

#endif // MAINWINDOW_H
