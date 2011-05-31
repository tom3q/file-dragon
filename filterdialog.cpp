#include <QSettings>
#include "filterdialog.h"
#include "ui_filterdialog.h"
#include "filefilter.h"

FilterDialog::FilterDialog(QWidget *parent, FileTree *tree) :
	QDialog(parent),
    ui(new Ui::FilterDialog)
{
	tree_ = tree;
    ui->setupUi(this);
	loadSettings();
}

FilterDialog::~FilterDialog()
{
	saveSettings();
    delete ui;
}

void FilterDialog::saveSettings()
{
	QSettings settings;
	settings.setValue("filterdialog/minenabled", ui->checkBox->isChecked());
	settings.setValue("filterdialog/maxenabled", ui->checkBox_2->isChecked());
	settings.setValue("filterdialog/minvalue", ui->lineEdit->text());
	settings.setValue("filterdialog/maxvalue", ui->lineEdit_2->text());
}

void FilterDialog::loadSettings()
{
	QSettings settings;
	bool minEnabled = settings.value("filterdialog/minenabled", false).toBool();
	bool maxEnabled = settings.value("filterdialog/maxenabled", false).toBool();
	QString minValue = settings.value("filterdialog/minvalue", "").toString();
	QString maxValue = settings.value("filterdialog/maxvalue", "").toString();

	ui->checkBox->setChecked( minEnabled );
	ui->lineEdit->setEnabled( minEnabled );
	ui->lineEdit->setText( minValue );
	ui->checkBox_2->setChecked( maxEnabled );
	ui->lineEdit_2->setEnabled( maxEnabled );
	ui->lineEdit_2->setText( maxValue );

	// set a filter for a FileTree
	on_pushButton_2_clicked();
}

void FilterDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
  * Called when state of 'minimum size' checkbox is changed.
  */
void FilterDialog::on_checkBox_stateChanged(int )
{
	ui->lineEdit->setEnabled( ui->checkBox->isChecked() );
}

/**
  * Called when state of 'maximum size' checkbox is changed.
  */
void FilterDialog::on_checkBox_2_stateChanged(int )
{
	ui->lineEdit_2->setEnabled( ui->checkBox_2->isChecked() );
}

/**
  * 'Cancel' button clicked. Hides the window.
  */
void FilterDialog::on_pushButton_clicked()
{
	hide();
}

/**
  * 'OK' button clicked. Creates a FileFilter based on the
	options defined by user. Applies the new filter to FileTree.
  */
void FilterDialog::on_pushButton_2_clicked()
{
	FileFilter filter;
	double value;

	// minimum size
	if (ui->checkBox->isChecked())
	{
		value = ui->lineEdit->text().toDouble();
		filter.setMinimumSize(value * 1024);
	}

	// maximum size
	if (ui->checkBox_2->isChecked())
	{
		value = ui->lineEdit_2->text().toDouble();
		filter.setMaximumSize(value * 1024);
	}

	hide();
	tree_->setFilter( filter );
}
