#include "m2mtilemesher_private.h"

#include "triplet.h"

using namespace M2M_Private;

M2M::PartialMeshData M2M_Private::polygonMesh(const BetterPolygon &polygon,
                                 float height,
                                 M2M::ImageInfo image,
                                 M2M::PhongInfo material)
{
    QList<Triplet<QPointF, QPointF, QPointF>> triangles = polygon.triangulate();

    M2M::PartialMeshData mesh;
    for (const Triplet<QPointF, QPointF, QPointF> &t : triangles) {
        QVector3D v1(t.getFirst().x(), height, t.getFirst().y());
        QVector3D v2(t.getSecond().x(), height, t.getSecond().y());
        QVector3D v3(t.getThird().x(), height, t.getThird().y());

        QVector2D t1(t.getFirst());
        QVector2D t2(t.getSecond());
        QVector2D t3(t.getThird());

        mesh.addTrig(M2M::Trig(image, material, v1, t1, v2, t2, v3, t3));
    }

    return mesh;
}

M2M::PartialMeshData M2M_Private::polygonSidesMesh(const BetterPolygon &polygon,
                                      QVector<bool> edgesToDrop,
                                      QVector<float> bottomHeight,
                                      float topHeight,
                                      M2M::ImageInfo image,
                                      M2M::PhongInfo material)
{
    M2M::PartialMeshData mesh;

    for (int i = 0; i < polygon.points().size(); ++i) {
        if (!edgesToDrop[i]) continue;
        int j = (i + 1) % polygon.points().size();

        QPointF a = polygon.points()[i];
        QPointF b = polygon.points()[j];
        QPointF xzCenter = (a + b) / 2;

        QVector2D dir(b - a);
        QVector2D normal(-dir.y(), dir.x());
        normal.normalize();
        QVector3D center(xzCenter.x(), (topHeight + bottomHeight[i]) / 2, xzCenter.y());

        float height = topHeight - bottomHeight[i];
        bool upsideDown = false;
        if (height == 0) continue;
        if (height < 0) {
            height = -height;
            upsideDown = true;
        }

        mesh.addQuad(M2M::Quad::makeVerticalQuad(center,
                                                 normal,
                                                 dir.length(),
                                                 height,
                                                 image,
                                                 material,
                                                 upsideDown));
    }

    return mesh;
}


QPoint SideTools::neighborOffset(SideDirection side)
{
    switch (side) {
    case NORTH: return QPoint(0, -1);
    case EAST:  return QPoint(1, 0);
    case SOUTH: return QPoint(0, 1);
    case WEST:  return QPoint(-1, 0);
    default:
        Q_ASSERT( false ); // there should only be 4 side directions
    }
}

QVector2D SideTools::awayDirection(SideDirection side)
{
    switch (side) {
    case NORTH: return QVector2D(0, -1);
    case EAST:  return QVector2D(1, 0);
    case SOUTH: return QVector2D(0, 1);
    case WEST:  return QVector2D(-1, 0);
    default:
        Q_ASSERT( false ); // there should only be 4 side directions
    }
}

QVector3D SideTools::normalDirection(SideDirection side)
{
    QVector2D away = SideTools::awayDirection(side);

    return QVector3D(away.x(), 0, away.y());
}


M2M::Quad SideTools::makeTopQuadFragment(SideDirection side,
                                         QVector2D quadCenter,
                                         QVector2D minusQuadCenter,
                                         float minusQuadSize,
                                         M2M::ImageInfo quadImage,
                                         M2M::PhongInfo quadMaterial)
{
    // The normal direction is just the away direction.
    QVector3D awayDirection = SideTools::normalDirection(side);

    // 'awayDirection' rotated CCW in the XZ plane looking down along Y axis.
    QVector3D leftDirection(awayDirection.z(), 0, -awayDirection.x());

    QVector3D mainCenter(quadCenter.x(), 0, quadCenter.y());
    QVector3D subCenter(minusQuadCenter.x(), 0, minusQuadCenter.y());

    QVector3D v1 = mainCenter + awayDirection * 0.5 - leftDirection * 0.5;
    QVector3D v2 = mainCenter + awayDirection * 0.5 + leftDirection * 0.5;
    QVector3D v3 = subCenter + (awayDirection + leftDirection) * 0.5 * minusQuadSize;
    QVector3D v4 = subCenter + (awayDirection - leftDirection) * 0.5 * minusQuadSize;

    // Away and left in tile coordinates.
    QVector2D tileAway(-awayDirection.x(), awayDirection.z());
    QVector2D tileLeft(-tileAway.y(), tileAway.x());

    // minusQuadCenter - quadCenter in tile coordinates.
    QVector2D tileMinusOffset(quadCenter.x() - minusQuadCenter.x(), minusQuadCenter.y() - quadCenter.y());

    QVector2D t1 = QVector2D(0.5, 0.5) + (tileAway - tileLeft) * 0.5;
    QVector2D t2 = QVector2D(0.5, 0.5) + (tileAway + tileLeft) * 0.5;
    QVector2D t3 = QVector2D(0.5, 0.5) + tileMinusOffset + (tileAway + tileLeft) * 0.5 * minusQuadSize;
    QVector2D t4 = QVector2D(0.5, 0.5) + tileMinusOffset + (tileAway - tileLeft) * 0.5 * minusQuadSize;

    return M2M::Quad(QVector3D(0, 1, 0),
                     quadImage,
                     quadMaterial,
                     v1, t1,
                     v2, t2,
                     v3, t3,
                     v4, t4);
}
