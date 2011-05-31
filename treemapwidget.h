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

	const set<FileNode*> getSelectedFiles() const;

	void removeSelection();

	void setMarkedFiles(set<FileNode*> s);

	/**
	* Checks if user selected a given file.
	*	@param node FileNode pointer.
	*	@return True if the file is selected, False if not.
	*/
	bool isSelected(FileNode *node) const;

	void setLegendVisible(bool show);
	bool isLegendVisible() const;

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
		void rootChanged(const QString &);

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
	class SortFunc
	{
	public:
		inline bool operator()(AbstractNode *i, AbstractNode *j)
		{
			return i->getSize() > j->getSize();
		}
	};

	void drawVert(QPainter &painter, QRectF &rect, set<AbstractNode *, SortFunc> &children, DetectionNode *node);
	void drawHorz(QPainter &painter, QRectF &rect, set<AbstractNode *, SortFunc> &children, DetectionNode *node);
	float worstHorz(set<AbstractNode*, SortFunc> &l, double &sum, double &dirSize, QRectF &r);
	float worstVert(set<AbstractNode*, SortFunc> &l, double &sum, double &dirSize, QRectF &r);
	float listSum(set<AbstractNode*, SortFunc> &l);

	FileNode *detectFile(int x, int y);
	DirectoryNode *detectDirectory(int x, int y);

	DetectionNode *detectRoot_;
	FileTree *tree_;
	DirectoryNode *currentRoot_;
	DefaultRenderer *renderer_;
	set<FileNode*> selectedNodes_;
	bool showLegend_;
};

#endif // TREEMAPWIDGET_H
