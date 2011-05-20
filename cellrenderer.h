#ifndef CELLRENDERER_H
#define CELLRENDERER_H

#include <QtGui>

#include "filenode.h"

class CellRenderer
{
public:
    CellRenderer();

    void render(QPainter &painter, QRectF rect, FileNode *file);
};

#endif // CELLRENDERER_H
