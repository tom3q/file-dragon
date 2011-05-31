#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemapwidget.h"
#include "filefilter.h"
#include "osoperations.h"
#include "treemanager.h"
#include "filemanager.h"

/**
  * Main window constructor.
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	QCoreApplication::setOrganizationName("Tolga");
	QCoreApplication::setApplicationName("File Dragon");

    // Add menus and other shit...
    createActions();
    createMenus();

    // Add a TreemapWidget to a form
    treemap = new TreemapWidget(this);
	ui->verticalLayout->addWidget(treemap);
    treemap->show();
	connect(treemap, SIGNAL(fileSelected(FileNode*)), this, SLOT(fileClicked(FileNode*)));

	// Frame containing file information
	fileFrame = new FileFrame(this);
	ui->verticalLayout->addWidget(fileFrame);
	fileFrame->show();
    
	// Create a filter dialog
	filterDialog = new FilterDialog(this, &treemap->getFileTree());

	// Create coloring dialog
	coloringDialog = new ColoringDialog(this, treemap);

	// Create file info dialog
	fileinfoDialog = new FileInfoDialog(this, fileFrame);

	// Create tree manager thread
	treeManager = new TreeManager(treemap->getFileTree());

	// Create file manager thread and dialog
	fileManager = new FileManager;
	queueDialog = new QueueDialog(this, fileManager);
	connect(actApply, SIGNAL(triggered()), this, SLOT(applyClicked()));

	// Create event chain for scan action
	connect(actScan, SIGNAL(triggered()), this, SLOT(scanClicked()));
	connect(this, SIGNAL(buildTree()), treeManager, SLOT(buildTree()), Qt::QueuedConnection);

	// Create event chain for scan callback
	connect(treeManager, SIGNAL(treeUpdated()), this, SLOT(scanDone()));
	connect(this, SIGNAL(refreshTreemap()), treemap, SLOT(fileTreeUpdated()));
	
	// Connect partition combo box directly with tree manager
	connect(comboPartition, SIGNAL(currentIndexChanged(const QString &)), treeManager, SLOT(setRootPath(const QString &)), Qt::QueuedConnection);

	// Connect cancel action to its handler
	connect(actCancel, SIGNAL(triggered()), this, SLOT(cancelClicked()));

	// Fill partition list (also sends currentIndexChanges signal to treeManager)
	fillComboPartition();

	// Progressbar
	scanProgress = new QProgressBar();
	scanProgress->setRange(0, 100);
	connect(treeManager, SIGNAL(progressUpdated(int)), scanProgress, SLOT(setValue(int)));

	// Create a label for displaying currently scanned directory
	nowScanningLabel = new QLabel();
	nowScanningLabel->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed));
	connect(treeManager, SIGNAL(nowScanning(const QString &)), nowScanningLabel, SLOT(setText(const QString &)));

	// Progress animation
	progressAnim = new QMovie(":/loading-gif-animation.gif");
	progressAnimLabel = new QLabel();
	progressAnimLabel->setMovie(progressAnim);
	progressAnimLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	progressAnim->setCacheMode(QMovie::CacheAll);

	// Progress bar layout and widget
	progressBarLayout = new QVBoxLayout();
	progressBarLayout->addWidget(progressAnimLabel);
	progressBarLayout->addWidget(nowScanningLabel);
	progressBarLayout->addWidget(scanProgress);
	progressBar = new QWidget();
	progressBar->setLayout(progressBarLayout);
	progressBar->hide();

	// Connect back action to treemap widget
	connect(actBack, SIGNAL(triggered()), treemap, SLOT(back()));

	// Create a label on status bar to show current path
	rootPathLabel = new QLabel(ui->statusBar);
	rootPathLabel->setText(tr("Select a disk to scan and click Scan button."));
	ui->statusBar->addWidget(rootPathLabel);

	// Connect treemap to the label to signal path changes
	connect(treemap, SIGNAL(rootChanged(const QString &)), rootPathLabel, SLOT(setText(const QString &)));

	//
	connect(actDelete, SIGNAL(triggered()), this, SLOT(deleteClicked()));

	//
	connect(queueDialog, SIGNAL(accepted()), this, SLOT(queueAccepted()));
	connect(this, SIGNAL(flushQueue()), fileManager, SLOT(flush()), Qt::QueuedConnection);

	connect(fileManager, SIGNAL(nowRemoving(const QString &)), nowScanningLabel, SLOT(setText(const QString &)));
	connect(fileManager, SIGNAL(progressUpdated(int)), scanProgress, SLOT(setValue(int)));

	connect(fileManager, SIGNAL(done()), this, SLOT(flushDone()));

	// restore program configuration
	loadSettings();
}

/**
  * Destructor
  */
MainWindow::~MainWindow()
{
	delete fileManager;
	delete treeManager;
    delete ui;

	delete filterDialog;
	delete fileinfoDialog;
	delete coloringDialog;
	
	delete progressBar;
	delete progressAnim;

	delete treemap;
	delete fileFrame;
    delete actScan;
    delete actUndo;
    delete actRedo;
    delete actApply;
	delete actCancel;
	delete actBack;
	delete actDelete;
    delete comboPartition;
    delete stretchWidget;
	delete rootPathLabel;
}

void MainWindow::saveSettings()
{
	QSettings settings;
	settings.setValue("mainwindow/size", size());
	settings.setValue("mainwindow/pos", pos());
	settings.setValue("mainwindow/showlegend", ui->actionShow_legend->isChecked());
}

void MainWindow::loadSettings()
{
	QSettings settings;
	QSize s = settings.value("mainwindow/size", QSize(640, 480)).toSize();
	QPoint point = settings.value("mainwindow/pos", QPoint(100, 100)).toPoint();
	bool showLegend = settings.value("mainwindow/showlegend", false).toBool();

	resize(s);
	move(point);
	ui->actionShow_legend->setChecked( showLegend );
	treemap->setLegendVisible( showLegend );
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

	actBack = new QAction(tr("Back"), this);
	actBack->setStatusTip(tr("Returns to the parent directory"));

	actDelete = new QAction(tr("Delete"), this);
	actDelete->setStatusTip(tr("Queues selected files for deletion"));
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
	ui->mainToolBar->addAction(actDelete);
    ui->mainToolBar->addAction(actApply);
	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addAction(actBack);

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
	fileFrame->updateInfo(0);
	actApply->setEnabled(false);
	actDelete->setEnabled(false);
	actScan->setEnabled(false);
	actCancel->setEnabled(true);
	comboPartition->setEnabled(false);
	treemap->hide();
	fileFrame->hide();
	ui->verticalLayout->removeWidget(fileFrame);
	ui->verticalLayout->removeWidget(treemap);
	ui->verticalLayout->addWidget(progressBar);
	rootPathLabel->setText(tr("Scanning..."));
	progressAnim->start();
	progressBar->show();

    emit buildTree();
}

/**
 *	Called when the tree gets built.
 */
void MainWindow::scanDone()
{
	emit refreshTreemap();

	progressBar->hide();
	progressAnim->stop();
	ui->verticalLayout->removeWidget(progressBar);
	ui->verticalLayout->addWidget(treemap);
	ui->verticalLayout->addWidget(fileFrame);
	treemap->show();
	fileFrame->show();
	comboPartition->setEnabled(true);
	actScan->setEnabled(true);
	actCancel->setEnabled(false);
	actApply->setEnabled(true);
	actDelete->setEnabled(true);
}

void MainWindow::cancelClicked()
{
	treeManager->cancel();
	fileManager->cancel();
}

void MainWindow::fileClicked(FileNode *file)
{
	fileFrame->updateInfo(file);
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

void MainWindow::closeEvent(QCloseEvent *)
{
	saveSettings();
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
	treemap->setLegendVisible(ui->actionShow_legend->isChecked());
}

void MainWindow::on_actionFile_information_triggered()
{
	fileinfoDialog->show();
}

void MainWindow::applyClicked()
{
	queueDialog->show();
}

void MainWindow::deleteClicked()
{
	fileManager->queueFiles(treemap->getSelectedFiles());
	treemap->removeSelection();
}

void MainWindow::queueAccepted()
{
	fileFrame->updateInfo(0);
	actApply->setEnabled(false);
	actDelete->setEnabled(false);
	actScan->setEnabled(false);
	actCancel->setEnabled(true);
	comboPartition->setEnabled(false);
	treemap->hide();
	fileFrame->hide();
	ui->verticalLayout->removeWidget(fileFrame);
	ui->verticalLayout->removeWidget(treemap);
	ui->verticalLayout->addWidget(progressBar);
	rootPathLabel->setText(tr("Removing..."));
	progressAnim->start();
	progressBar->show();

    emit flushQueue();
}

void MainWindow::flushDone()
{
	rootPathLabel->setText(tr("Scanning..."));

	emit buildTree();
}
