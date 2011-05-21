#ifndef TREEMAPWIDGET_H
#define TREEMAPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRect>

#include "filetree.h"
#include "cellrenderer.h"

class TreemapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TreemapWidget(QWidget *parent = 0);
    ~TreemapWidget();

    void setCellRenderer(CellRenderer *);
    FileTree &getFileTree() const;

public slots:
    void fileTreeUpdated();

protected:
    void paintEvent(QPaintEvent *event);

private:
    void drawDirVert(QPainter &painter, QRectF &rect, DirectoryNode *dir);
    void drawDirHorz(QPainter &painter, QRectF &rect, DirectoryNode *dir);

    FileTree *tree_;
    CellRenderer *renderer_;
};

#endif // TREEMAPWIDGET_H
