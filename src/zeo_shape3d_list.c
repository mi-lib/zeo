/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_list - 3D shape: shape list.
 */

#include <zeo/zeo_shape3d.h>

/* push a shape to a list. */
zShape3DListCell *zShape3DListPush(zShape3DList *list, zShape3D *shape)
{
  zShape3DListCell *cp;

  if( !( cp = zAlloc( zShape3DListCell, 1 ) ) ) return NULL;
  zShape3DListCellSetShape( cp, shape );
  zListInsertTail( list, cp );
  return cp;
}

/* pop a shape from a list. */
zShape3D *zShape3DListPop(zShape3DList *list)
{
  zShape3DListCell *cp;
  zShape3D *shape;

  if( zListIsEmpty(list) ) return NULL;
  zListDeleteTail( list, &cp );
  shape = zShape3DListCellShape( cp );
  zFree( cp );
  return shape;
}

/* contiguous vertix of shapes in a list to a point. */
const zVec3D *zShape3DListContigVert(const zShape3DList *list, const zVec3D *point, double *distance)
{
  zShape3DListCell *cp;
  const zVec3D *v, *nv;
  double _d, dist_min;

  if( !distance ) distance = &_d;
  cp = zListTail( list );
  v = zShape3DContigVert( zShape3DListCellShape(cp), point, distance );
  dist_min = *distance;
  for( cp=zListCellNext(cp); cp!=zListRoot(list); cp=zListCellNext(cp) ){
    nv = zShape3DContigVert( zShape3DListCellShape(cp), point, distance );
    if( *distance < dist_min ){
      v = nv;
      dist_min = *distance;
    }
  }
  *distance = dist_min;
  return v;
}

/* the closest point to a shape in a list. */
double zShape3DListClosest(const zShape3DList *list, const zVec3D *point, zVec3D *closestpoint)
{
  zShape3DListCell *c;
  zVec3D cp;
  double dist, dist_min;

  c = zListTail( list );
  dist_min = zShape3DClosest( zShape3DListCellShape(c), point, closestpoint );
  for( c=zListCellNext(c); c!=zListRoot(list); c=zListCellNext(c) )
    if( ( dist = zShape3DClosest( zShape3DListCellShape(c), point, &cp ) ) < dist_min ){
      zVec3DCopy( &cp, closestpoint );
      dist_min = dist;
    }
  return dist_min;
}

/* print a cell of a shape list out to a file in a ZTK format. */
void zShape3DListCellFPrintZTK(FILE *fp, const zShape3DListCell *cell)
{
  if( !cell || !zShape3DListCellShape(cell) ) return;
  zShape3DFPrintZTK( fp, zShape3DListCellShape(cell) );
}

/* print a list of shapes to a file in a ZTK format. */
void zShape3DListFPrintZTK(FILE *fp, const zShape3DList *list)
{
  zShape3DListCell *cp;

  zListForEach( list, cp )
    zShape3DListCellFPrintZTK( fp, cp );
}
