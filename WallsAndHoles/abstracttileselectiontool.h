#ifndef ABSTRACTTILESELECTIONTOOL_H
#define ABSTRACTTILESELECTIONTOOL_H

#include "abstracttilemaptool.h"
#include "propertybrowser.h"

class AbstractTileSelectionTool : public AbstractTileMapTool
{
public:
    AbstractTileSelectionTool(PropertyBrowser *propertyBrowser,
                              TileMapPreviewGraphicsItem *previewItem);

    static const QRegion &selection() { return mSelection; }

    void deactivate() override;

protected:
    /**
     * @brief activateSelection
     *
     * After updating mSelection, calling this will perform all expected actions,
     * such as updating the graphics and property browser.
     */
    void activateSelection();
    void drawPreview();

    //all selection tools should share the same selection region.
    static QRegion mSelection;

    PropertyBrowser *mPropertyBrowser;
};

#endif // ABSTRACTTILESELECTIONTOOL_H
