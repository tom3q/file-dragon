#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    void createActions();
    void createMenus();
    void fillComboPartition();

    FileTree *tree;
    Ui::MainWindow *ui;
    TreemapWidget *treemap;
    QComboBox *comboPartition;
    QLabel *stretchWidget;
    QAction *actScan;
    QAction *actUndo;
    QAction *actRedo;
    QAction *actApply;
};

#endif // MAINWINDOW_H
