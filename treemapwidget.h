#ifndef TREEMAPWIDGET_H
#define TREEMAPWIDGET_H

#include <QWidget>
#include "filetree.h"

class TreemapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TreemapWidget(QWidget *parent = 0);
    ~TreemapWidget();

    FileTree &getFileTree();

public slots:
    void fileTreeUpdated();

protected:
    void paintEvent(QPaintEvent *event);

private:
    FileTree *tree_;
};

#endif // TREEMAPWIDGET_H
