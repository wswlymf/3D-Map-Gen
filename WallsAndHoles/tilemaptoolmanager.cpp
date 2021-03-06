#include "tilemaptoolmanager.h"

inline AbstractTileMapTool *tool2TileMapTool(AbstractTool *tool)
{
    return static_cast<AbstractTileMapTool *>(tool);
}

TileMapToolManager::TileMapToolManager(QObject *parent)
    : ToolManager(parent) {}


QAction *TileMapToolManager::registerMapTool(AbstractTileMapTool *tool,
                                             QString name,
                                             QIcon icon,
                                             QKeySequence ks)
{
    return ToolManager::registerTool(tool, name, icon, ks);
}

void TileMapToolManager::setTileMap(TileMap *tileMap)
{
    for (AbstractTool *tool : mTools)
        tool2TileMapTool(tool)->setTileMap(tileMap);
}

void TileMapToolManager::cellActivated(int x, int y, QMouseEvent *event)
{
    if (mActiveTool != nullptr) tool2TileMapTool(mActiveTool)->cellActivated(x, y, event);
}

void TileMapToolManager::cellClicked(int x, int y, QMouseEvent *event)
{
    if (mActiveTool != nullptr) tool2TileMapTool(mActiveTool)->cellClicked(x, y, event);
}

void TileMapToolManager::cellReleased(int x, int y, QMouseEvent *event)
{
    if (mActiveTool != nullptr) tool2TileMapTool(mActiveTool)->cellReleased(x, y, event);
}

void TileMapToolManager::cellHovered(int x, int y, QMouseEvent *event)
{
    if (mActiveTool != nullptr) tool2TileMapTool(mActiveTool)->cellHovered(x, y, event);
}

void TileMapToolManager::mouseExitedMap(QMouseEvent *event)
{
    if (mActiveTool != nullptr) tool2TileMapTool(mActiveTool)->mouseExitedMap(event);
}

void TileMapToolManager::tileTemplateChanged(TileTemplate *tileTemplate)
{
    for (AbstractTool *tool : mTools)
        tool2TileMapTool(tool)->setTileTemplate(tileTemplate);
}
