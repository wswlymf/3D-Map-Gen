#include "abstractshapebrushtool.h"

AbstractShapeBrushTool::AbstractShapeBrushTool(TileMapPreviewGraphicsItem *previewItem)
    : AbstractTileMapTool(previewItem) {}

void AbstractShapeBrushTool::cellClicked(int x, int y, QMouseEvent *)
{
    mStartX = x;
    mStartY = y;

    drawOverlay(x, y);
}


void AbstractShapeBrushTool::cellActivated(int x, int y, QMouseEvent *)
{
    drawOverlay(x, y);
}


void AbstractShapeBrushTool::cellReleased(int x, int y, QMouseEvent *)
{
    clearOverlay();
    placeShape(x, y);
}

void AbstractShapeBrushTool::deactivate()
{
    clearOverlay();
}


void AbstractShapeBrushTool::drawOverlay(int endX, int endY) {
    clearOverlay();

    QRegion region = getShape(QPoint(mStartX, mStartY), QPoint(endX, endY));

    mPreviewItem->setRegion(region);
    if (TileTemplate *t = getTileTemplate())
        mPreviewItem->setColor(t->color());
    else
        mPreviewItem->setColor(Qt::gray);
}

void AbstractShapeBrushTool::clearOverlay() {
    // Clears overlay.
    mPreviewItem->setRegion(QRegion());
}

void AbstractShapeBrushTool::placeShape(int endX, int endY) {
    QRegion region = getShape(QPoint(mStartX, mStartY), QPoint(endX, endY));

    for (const QRect &r : region)
        for (int x = r.left(); x <= r.right(); ++x)
            for (int y = r.top(); y <= r.bottom(); ++y)
                if (getTileMap()->contains(x, y))
                    getTileMap()->setTile(x, y, getTileTemplate());
}
