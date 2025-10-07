/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_list - 3D shape: shape list
 */

#include <zeo/zeo_shape3d.h>

/* ********************************************************** */
/* CLASS: zShapeListCell & zShapeList
 * shape list class
 * ********************************************************** */

/* push a shape to a list. */
zShapeListCell *zShapeListPush(zShapeList *list, zShape3D *shape)
{
  zShapeListCell *cp;

  if( !( cp = zAlloc( zShapeListCell, 1 ) ) ) return NULL;
  zShapeListCellSetShape( cp, shape );
  zListInsertTail( list, cp );
  return cp;
}

/* pop a shape from a list. */
zShape3D *zShapeListPop(zShapeList *list)
{
  zShapeListCell *cp;
  zShape3D *shape;

  if( zListIsEmpty(list) ) return NULL;
  zListDeleteTail( list, &cp );
  shape = zShapeListCellShape( cp );
  zFree( cp );
  return shape;
}

/* contiguous vertix of shapes in a list to a point. */
const zVec3D *zShapeListContigVert(const zShapeList *list, const zVec3D *point, double *distance)
{
  zShapeListCell *cp;
  const zVec3D *v, *nv;
  double _d, dist_min;

  if( !distance ) distance = &_d;
  cp = zListTail( list );
  v = zShape3DContigVert( zShapeListCellShape(cp), point, distance );
  dist_min = *distance;
  for( cp=zListCellNext(cp); cp!=zListRoot(list); cp=zListCellNext(cp) ){
    nv = zShape3DContigVert( zShapeListCellShape(cp), point, distance );
    if( *distance < dist_min ){
      v = nv;
      dist_min = *distance;
    }
  }
  *distance = dist_min;
  return v;
}

/* the closest point to a shape in a list. */
double zShapeListClosest(const zShapeList *list, const zVec3D *point, zVec3D *closestpoint)
{
  zShapeListCell *c;
  zVec3D cp;
  double dist, dist_min;

  c = zListTail( list );
  dist_min = zShape3DClosest( zShapeListCellShape(c), point, closestpoint );
  for( c=zListCellNext(c); c!=zListRoot(list); c=zListCellNext(c) )
    if( ( dist = zShape3DClosest( zShapeListCellShape(c), point, &cp ) ) < dist_min ){
      zVec3DCopy( &cp, closestpoint );
      dist_min = dist;
    }
  return dist_min;
}

/* print a cell of a shape list out to a file in a ZTK format. */
void zShapeListCellFPrintZTK(FILE *fp, const zShapeListCell *cell)
{
  if( !cell || !zShapeListCellShape(cell) ) return;
  zShape3DFPrintZTK( fp, zShapeListCellShape(cell) );
}

/* print a list of shapes to a file in a ZTK format. */
void zShapeListFPrintZTK(FILE *fp, const zShapeList *list)
{
  zShapeListCell *cp;

  zListForEach( list, cp )
    zShapeListCellFPrintZTK( fp, cp );
}
