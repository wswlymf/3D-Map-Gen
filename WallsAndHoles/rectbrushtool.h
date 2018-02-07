#ifndef RECTBRUSHTOOL_H
#define RECTBRUSHTOOL_H


#include "abstractshapebrushtool.h"

class RectBrushTool : public AbstractShapeBrushTool {
public:
    RectBrushTool(MapView *mapView, TileMap *tileMap, SharedTileTemplate drawWith);

    /// @brief Draws a rectangle.
    QVector<QPoint> getShape(int dx, int dy) const override;
};

#endif // RECTBRUSHTOOL_H