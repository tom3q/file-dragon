/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T�kiel
*
*	treemapwidget.h
*	TreemapWidget class prototype.
*/

#ifndef TREEMAPWIDGET_H
#define TREEMAPWIDGET_H

#ifdef __linux__
#define USE_OPENGL
#endif

#ifdef USE_OPENGL
#include <QGLWidget>
#else
#include <QWidget>
#endif
#include <QPainter>
#include <QRect>
#include <set>

#include "filetree.h"
#include "defaultrenderer.h"
#include "detectionnode.h"

using namespace std;

/**
 * TreemapWidget is a class which implements a Qt widget for drawing
 * a squarified treemap based on hierarchical data.
 */
#ifdef USE_OPENGL
class TreemapWidget : public QGLWidget
#else
class TreemapWidget : public QWidget
#endif
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
	 * each cell during paint event using its render() method and
	 * file legend if such option is used.
	 * @param cr Pointer to a CellRenderer object.
	 */
	void setRenderer(AbstractRenderer *cr);

	/**
	 * Returns a reference to its FileTree object.
	 * @return A Filetree reference.
	 */
	FileTree &getFileTree() const;

	/**
	 * Returns a number of files mouse-selected by user.
	 * @return Number of selected files.
	 */
	int getSelectedCount() const;

	/**
	 * Returns pointer to one of the selected nodes by
	 * the given index.
	 * @param index Index of a selected node.
	 * @return FileNode pointer defined by index.
	 */
	FileNode *getSelected(int index);


	const set<FileNode*> getSelectedFiles() const;

	/**
	 * Removes selection.
	 */
	void removeSelection();

	void setMarkedFiles(set<FileNode*> s);

	/**
	 * Checks if user selected a given file.
	 * @param node FileNode pointer.
	 * @return True if the file is selected, False if not.
	 */
	bool isSelected(FileNode *node) const;

	/**
	 * Shows or hides the legend.
	 * @param show True to show the legend.
	 */
	void setLegendVisible(bool show);
	/**
	 * Checks whether the legend is visible.
	 * @return True if the legend is visible.
	 */
	bool isLegendVisible() const;
	/**
	 * Returns the preferred widget size.
	 * @return Preferred widget size.
	 */
	QSize sizeHint() const;

public slots:
	/**
	 * This slot triggers the widget to reparse the underlying tree
	 * and redraw itself.
	 */
	void fileTreeUpdated();
	/**
	 * This slot triggers the widget to go up one level in the tree.
	 */
	void back();

signals:
	/**
	 * This signal is emitted when a file is selected.
	 * @param FileNode A FileNode describing the selected file.
	 */
	void fileSelected(FileNode *);
	/**
	 * This signal is emitted when some user action (e.g. double click)
	 * changes current root.
	 */
	void rootChanged(const QString &);

protected:
	/**
	 * In this event all painting should take place.
	 */
	void paintEvent(QPaintEvent *event);

	/**
	 * Called when a user clicks the widget. Checks which file was
	 * clicked and selects it. If Control key is held down, allows
	 * to select more than one file.
	 */
	void mousePressEvent(QMouseEvent *event);

	/**
	 * Called when a user double clisk the event. Causes the widget
	 * to move to the subtree when the file is located.
	 */
	void mouseDoubleClickEvent(QMouseEvent *event);

private:
	class SortFunc
	{
	public:
		inline bool operator()(AbstractNode *i, AbstractNode *j)
		{
			return i->getSize() > j->getSize();
		}
	};

	void drawVert(QPainter &painter, QRectF &rect, multiset<AbstractNode *, SortFunc> &children, DetectionNode *node);
	void drawHorz(QPainter &painter, QRectF &rect, multiset<AbstractNode *, SortFunc> &children, DetectionNode *node);
	float worstHorz(multiset<AbstractNode *, SortFunc> &l, double &sum, double &dirSize, QRectF &r);
	float worstVert(multiset<AbstractNode *, SortFunc> &l, double &sum, double &dirSize, QRectF &r);
	float listSum(multiset<AbstractNode *, SortFunc> &l);

	FileNode *detectFile(int x, int y);
	DirectoryNode *detectDirectory(int x, int y);

	DetectionNode *detectRoot_;
	FileTree *tree_;
	DirectoryNode *currentRoot_;
	AbstractRenderer *renderer_;
	set<FileNode*> selectedNodes_;
	bool showLegend_;
};

#endif // TREEMAPWIDGET_H
