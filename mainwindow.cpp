#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemapwidget.h"
#include "filefilter.h"

/**
  * Main window constructor.
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Add menus and other shit...
    createActions();
    createMenus();

    // Add a TreemapWidget to a form
    treemap = new TreemapWidget(this);
    ui->horizontalLayout->addWidget(treemap);
    treemap->show();
}

/**
  * Destructor
  */
MainWindow::~MainWindow()
{
    delete treemap;
    delete ui;
}

/**
  * Creates menu/toolbar actions, which will be used in the application.
  */
void MainWindow::createActions()
{
    actScan = new QAction(tr("Scan"), this);
    actScan->setStatusTip(tr("Scans a selected partition"));
    connect(actScan, SIGNAL(triggered()), this, SLOT(scanClicked()));
}

/**
  * Adds separators/actions to menus and toolbar.
  */
void MainWindow::createMenus()
{
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(actScan);
}

/**
  * Called when user clicks "Scan" toolbar button.
  */
void MainWindow::scanClicked()
{
    FileTree &tree = treemap->getFileTree();
    tree.buildTree("/media/disc2/Obrazy");

    FileFilter filter;
    filter.setMaximumSize(1024 * 500);
    tree.filter(filter);

    tree.printFiles();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
