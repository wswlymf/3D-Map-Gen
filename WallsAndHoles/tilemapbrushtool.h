#ifndef TILEMAPBRUSHTOOL_H
#define TILEMAPBRUSHTOOL_H

#include "abstracttilemaptool.h"
#include "tilemap.h"
#include "tiletemplate.h"

#include <QWidgetAction>

class TileMapBrushTool : public AbstractTileMapTool
{
public:
    TileMapBrushTool(TileMapPreviewGraphicsItem *previewItem);

    void cellActivated(int x, int y, QMouseEvent *) override;
    void cellHovered(int x, int y, QMouseEvent *) override;
    void mouseExitedMap(QMouseEvent *);

    QList<QAction *> contextActions() override { return {mRadiusSpinner}; }

private:
    int mRadius;

    QWidgetAction *mRadiusSpinner;
};

#endif // TILEMAPBRUSHTOOL_H
