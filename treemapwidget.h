#ifndef TREEMAPWIDGET_H
#define TREEMAPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRect>
#include <set>

#include "filetree.h"
#include "defaultrenderer.h"

using namespace std;

class TreemapWidget : public QWidget
{
    Q_OBJECT
public:
	const static int LEGEND_HEIGHT = 30;
	const static int LEGEND_MARGIN = 10;

	/**
	  * Widget constructor.
	  */
    explicit TreemapWidget(QWidget *parent = 0);

	/**
	  * Widget destructor.
	  */
    ~TreemapWidget();

	/**
	  * Sets a renderer for a TreemapWidget. DefaultRenderer renders
		each cell during paint event using its render() method and
		file legend if such option is used.
		@param cr Pointer to a CellRenderer object.
	  */
	void setRenderer(DefaultRenderer *cr);

	/**
	  * Returns a reference to its FileTree object.
	  * @return A Filetree reference.
	  */
    FileTree &getFileTree() const;

	/**
	  * Returns a number of files mouse-selected by user.
	  *	@return Number of selected files.
	  */
	int getSelectedCount() const;

	/**
	  * Returns pointer to one of the selected nodes by
		the given index.
	  *	@param index Index of a selected node.
	  * @return FileNode pointer defined by index.
	  */
	FileNode *getSelected(int index);

	/**
	  * Checks if user selected a given file.
	  *	@param node FileNode pointer.
	  *	@return True if the file is selected, False if not.
	  */
	bool isSelected(FileNode *node) const;

	void setShowLegend(bool show);

	QSize sizeHint() const;

public slots:
	/**
	  * This slot is called when FileTree changes its contents.
		Clears selection and redraws the widget.
	  */
    void fileTreeUpdated();
	void back();

signals:

	void fileSelected(FileNode *);

protected:
	/**
	  * In this event all painting should take place.
	  */
	void paintEvent(QPaintEvent *event);

	/**
	  * Called when a user clicks the widget. Checks which file was
		clicked and selects it. If Control key is held down, allows
		to select more than one file.
	  */
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

private:
    void drawDirVert(QPainter &painter, QRectF &rect, DirectoryNode *dir);
    void drawDirHorz(QPainter &painter, QRectF &rect, DirectoryNode *dir);
	FileNode *detectFile(int, int);
	DirectoryNode *detectDirectory(int, int);

    FileTree *tree_;
	DirectoryNode *currentRoot_;
	DefaultRenderer *renderer_;
	set<FileNode*> selectedNodes_;
	bool showLegend_;
	bool startVert;
};

#endif // TREEMAPWIDGET_H
