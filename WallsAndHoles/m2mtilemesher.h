#ifndef M2MTILEMESHER_H
#define M2MTILEMESHER_H


#include <QVector2D>
#include <QVector3D>
#include <QSharedPointer>
#include <QImage>

#include "array2d.h"

#include "m2mtilemesher_private.h"
#include "m2mpartialmesh.h"

#include "tile.h"
#include "simpletexturedobject.h"

namespace M2M {

class AbstractTileMesher;


struct GroundInfo {
    float groundHeight;
    ImageInfo groundImage;
    PhongInfo groundMaterial;

    bool operator ==(const GroundInfo &other) const
    {
        return groundHeight == other.groundHeight
                && groundImage == other.groundImage
                && groundMaterial == other.groundMaterial;
    }

    bool operator !=(const GroundInfo &other) const
    {
        return !(*this == other);
    }
};


class TileInfo {
public:
    TileInfo();                     /// Creates a null tile info.
    TileInfo(const Tile &tile);     /// Creates a tile info for the given tile.


    bool isNull() const { return mIsNull; }
    bool isGround() const { return mIsGround; }

    float topHeight() const { return mTopHeight; }
    float thickness() const { return mThickness;}

    QVector2D offsetFromCenter() const { return mOffsetFromCenter; }

    GroundInfo groundInfo() const { return mGround; }

    ImageInfo topImage() const { return mTopImage; }
    ImageInfo sideImage() const { return mSideImage; }

    PhongInfo topMaterial() const { return mTopMaterial; }
    PhongInfo sideMaterial() const { return mSideMaterial; }


    bool operator ==(const TileInfo &other) const;
    bool operator !=(const TileInfo &other) const;

private:


    /// If this is true, the TileInfo object does not represent any tile.
    bool mIsNull;

    /// If this is true, then this is a ground tile.
    bool mIsGround;

    float mTopHeight;
    float mThickness;
    QVector2D mOffsetFromCenter;

    GroundInfo mGround;

    ImageInfo mTopImage;
    ImageInfo mSideImage;

    PhongInfo mTopMaterial;
    PhongInfo mSideMaterial;




    static SharedImageAndSource getDefaultImage();
    static SharedImageAndSource DefaultImage;
};



class TileNeighborhoodInfo {
public:

    TileNeighborhoodInfo(Array2D<const Tile *> nbhd);

    bool operator ==(const TileNeighborhoodInfo &other) const;
    bool operator !=(const TileNeighborhoodInfo &other) const;

    const TileInfo &operator ()(int x, int y) const;
    const TileInfo &operator ()(QPoint pt) const;

    const TileInfo &centerTile() const;

    QSharedPointer<AbstractTileMesher> makeMesher() const;


private:
    Array2D<TileInfo> mTileInfos;        /// Mesh-relevant information for every tile.

};


/**
 * @brief A collection of methods for generating tile mesh data.
 */
class AbstractTileMesher {
public:

    virtual ~AbstractTileMesher() {}

    /**
     * @brief Returns a TileMesher instance that will create the mesh for the given tile,
     * or, if the tile has not changed (determined by oldMesher), returns nullptr.
     */
    static QSharedPointer<AbstractTileMesher> getMesherForTile(
            Array2D<const Tile*> neighborhood,
            const AbstractTileMesher *oldMesher = nullptr);


    /**
     * @brief makeMesh  Creates the mesh for this tile.
     * @return          A list of objects that contain the mesh info for the tile.
     */
    virtual QVector<QSharedPointer<SimpleTexturedObject>> makeMesh(QVector2D offset) = 0;


protected:
    AbstractTileMesher(TileNeighborhoodInfo nbhd);

    /**
     * @brief Information for each tile in the 3x3 neighborhood of the tile to which this mesher is associated.
     */
    TileNeighborhoodInfo mTileNeighborhood;

    /**
     * @brief makeTopMesh           Creates the top quad for a tile.
     * @param meshData              The data will be put here.
     * @param offset                The offset of the tile.
     */
    void makeTopMesh(PartialMeshData &meshData,
                     QVector2D offset);


    /**
     * @brief makeVerticalSideMesh  Creates a vertical side for the tile.
     * @param meshData              The mesh goes into this variable.
     * @param sideDirection         Which side should be made.
     * @param offset                The offset of the tile.
     */
    void makeVerticalSideMesh(PartialMeshData &meshData,
                              M2M_Private::SideDirection sideDirection,
                              QVector2D offset);
};


class TileBlockyMesher : public AbstractTileMesher {
public:
    TileBlockyMesher(TileNeighborhoodInfo nbhd);

    QVector<QSharedPointer<SimpleTexturedObject>> makeMesh(QVector2D offset);
};

//class TileDiagonalMesher : public TileMesher {
//public:
//    TileDiagonalMesher(TileNeighborhoodInfo nbhd);

//protected:
//    /**
//     * @brief makeDiagonalSideMesh  Creates the diagonal mesh for a side. May create part of the bridge.
//     * @param meshData              Where to put the output.
//     * @param sideDirection         Which side to do this for.
//     * @param diagonalConnection    To which tile are we connecting diagonally? Should be in the square between (0,0) and (2,2).
//     */
//    void makeDiagonalSideMesh(PartialMeshData &meshData,
//                              Direction sideDirection,
//                              QPoint diagonalConnection);
//};



}

#endif // M2MTILEMESHER_H
