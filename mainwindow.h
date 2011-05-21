#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "filterdialog.h"
#include "treemapwidget.h"
#include "filetree.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void scanClicked();

protected:
    void changeEvent(QEvent *e);

private slots:
	void on_actionFilters_triggered();

private:
    void createActions();
    void createMenus();
    void fillComboPartition();

	Ui::MainWindow *ui;
	FilterDialog *filterDialog;
    FileTree *tree;
    TreemapWidget *treemap;
    QComboBox *comboPartition;
    QLabel *stretchWidget;
    QAction *actScan;
    QAction *actUndo;
    QAction *actRedo;
    QAction *actApply;
};

#endif // MAINWINDOW_H
