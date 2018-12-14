/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_list - 3D shape: shape list
 */

#include <zeo/zeo_shape.h>

/* ********************************************************** */
/* CLASS: zShapeListCell & zShapeList
 * shape list class
 * ********************************************************** */

/* zShapeListPush
 * - push of shape list.
 */
zShapeListCell *zShapeListPush(zShapeList *l, zShape3D *shape)
{
  zShapeListCell *cp;

  if( !( cp = zAlloc( zShapeListCell, 1 ) ) ) return NULL;
  zShapeListCellSetShape( cp, shape );
  zListInsertTail( l, cp );
  return cp;
}

/* zShapeListPop
 * - pop of shape list.
 */
zShape3D *zShapeListPop(zShapeList *l)
{
  zShapeListCell *cp;
  zShape3D *shape;

  if( zListIsEmpty(l) ) return NULL;
  zListDeleteTail( l, &cp );
  shape = zShapeListCellShape( cp );
  zFree( cp );
  return shape;
}

/* zShapeListContigVert
 * - contiguous vertix of shape list to a point.
 */
zVec3D *zShapeListContigVert(zShapeList *l, zVec3D *p, double *d)
{
  zShapeListCell *cp;
  zVec3D *v, *nv;
  double _d, dmin;

  if( !d ) d = &_d;
  cp = zListTail( l );
  v = zShape3DContigVert( zShapeListCellShape(cp), p, d );
  dmin = *d;
  for( cp=zListCellNext(cp); cp!=zListRoot(l); cp=zListCellNext(cp) ){
    nv = zShape3DContigVert( zShapeListCellShape(cp), p, d );
    if( *d < dmin ){
      v = nv;
      dmin = *d;
    }
  }
  *d = dmin;
  return v;
}

/* zShapeListClosest
 * - the closest point to shape list.
 */
double zShapeListClosest(zShapeList *l, zVec3D *p, zVec3D *cp)
{
  zShapeListCell *c;
  zVec3D ncp;
  double d, dmin;

  c = zListTail( l );
  dmin = zShape3DClosest( zShapeListCellShape(c), p, cp );
  for( c=zListCellNext(c); c!=zListRoot(l); c=zListCellNext(c) )
    if( ( d = zShape3DClosest( zShapeListCellShape(c), p, &ncp ) ) < dmin ){
      zVec3DCopy( &ncp, cp );
      dmin = d;
    }
  return dmin;
}

/* zShapeListCellFWrite
 * - output of shape list cell.
 */
void zShapeListCellFWrite(FILE *fp, zShapeListCell *cell)
{
  if( !cell || !zShapeListCellShape(cell) ) return;
  zShape3DFWrite( fp, zShapeListCellShape(cell) );
}

/* zShapeListFWrite
 * - output of shape list.
 */
void zShapeListFWrite(FILE *fp, zShapeList *list)
{
  zShapeListCell *cp;

  zListForEach( list, cp )
    zShapeListCellFWrite( fp, cp );
}
