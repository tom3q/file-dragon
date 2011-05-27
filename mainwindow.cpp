#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemapwidget.h"
#include "filefilter.h"
#include "osoperations.h"
#include "treemanager.h"

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
	ui->verticalLayout->addWidget(treemap);
    treemap->show();
    
	// Create a filter dialog
	filterDialog = new FilterDialog(this, &treemap->getFileTree());

	// Create coloring dialog
	coloringDialog = new ColoringDialog(this, treemap);
	// Create tree manager thread
	treeManager = new TreeManager(treemap->getFileTree());

	// Create event chain for scan action
	connect(actScan, SIGNAL(triggered()), this, SLOT(scanClicked()),  Qt::QueuedConnection);
	connect(this, SIGNAL(buildTree()), treeManager, SLOT(buildTree()), Qt::QueuedConnection);

	// Create event chain for scan callback
	connect(treeManager, SIGNAL(treeUpdated()), this, SLOT(scanDone()));
	connect(this, SIGNAL(refreshTreemap()), treemap, SLOT(fileTreeUpdated()));
	
	// Connect partition combo box directly with tree manager
	connect(comboPartition, SIGNAL(currentIndexChanged(const QString &)), treeManager, SLOT(setRootPath(const QString &)), Qt::QueuedConnection);

	// Connect cancel action to its handler
	connect(actCancel, SIGNAL(triggered()), this, SLOT(cancelClicked()), Qt::QueuedConnection);

	// Fill partition list (also sends currentIndexChanges signal to treeManager)
	fillComboPartition();
}

/**
  * Destructor
  */
MainWindow::~MainWindow()
{
	delete treeManager;
	delete filterDialog;
    delete treemap;
    delete ui;

    delete actScan;
    delete actUndo;
    delete actRedo;
    delete actApply;
	delete actCancel;
    delete comboPartition;
    delete stretchWidget;
}

/**
  * Creates menu/toolbar actions, which will be used in the application.
  */
void MainWindow::createActions()
{
    actScan = new QAction(tr("Scan"), this);
    actScan->setStatusTip(tr("Scans a selected partition"));

    actUndo = new QAction(tr("Undo"), this);
    actUndo->setStatusTip(tr("Undoes the file operation"));

    actRedo = new QAction(tr("Redo"), this);
    actRedo->setStatusTip(tr("Redoes a file operation"));

    actApply = new QAction(tr("Apply"), this);
    actApply->setStatusTip(tr("Executes all file operations"));

	actCancel = new QAction(tr("Cancel"), this);
	actCancel->setStatusTip(tr("Cancels pending operation"));
	actCancel->setEnabled(false);
}

/**
  * Adds separators/actions to menus and toolbar.
  */
void MainWindow::createMenus()
{
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(actScan);
	ui->mainToolBar->addAction(actCancel);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(actUndo);
    ui->mainToolBar->addAction(actRedo);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(actApply);

    stretchWidget = new QLabel(this);
    stretchWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->mainToolBar->addWidget(stretchWidget);
    stretchWidget->show();

    comboPartition = new QComboBox(this);
    ui->mainToolBar->addWidget(comboPartition);
    comboPartition->show();
}

void MainWindow::fillComboPartition()
{
    QStringList list = OSOperations::diskList();

    comboPartition->clear();
    for (int i=0; i<list.size(); i++)
        comboPartition->addItem(list.at(i));
}

/**
  * Called when user clicks "Scan" toolbar button.
  */
void MainWindow::scanClicked()
{
	actScan->setEnabled(false);
	actCancel->setEnabled(true);
	comboPartition->setEnabled(false);
	treemap->hide();
	// TODO: Prepare progress bar here.
    emit buildTree();
}

/**
 *	Called when the tree gets built.
 */
void MainWindow::scanDone()
{
	emit refreshTreemap();
	// TODO: Hide progress bar here.
	treemap->show();
	comboPartition->setEnabled(true);
	actScan->setEnabled(true);
	actCancel->setEnabled(false);
}

void MainWindow::cancelClicked()
{
	treeManager->cancel();
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

void MainWindow::on_actionFilters_triggered()
{
	filterDialog->show();
}

void MainWindow::on_actionCell_coloring_triggered()
{
	coloringDialog->show();
}

void MainWindow::on_actionShow_legend_changed()
{
	treemap->setShowLegend(ui->actionShow_legend->isChecked());
}
