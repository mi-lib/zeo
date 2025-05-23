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
zVec3D *zShapeListContigVert(const zShapeList *list, const zVec3D *p, double *d)
{
  zShapeListCell *cp;
  zVec3D *v, *nv;
  double _d, dmin;

  if( !d ) d = &_d;
  cp = zListTail( list );
  v = zShape3DContigVert( zShapeListCellShape(cp), p, d );
  dmin = *d;
  for( cp=zListCellNext(cp); cp!=zListRoot(list); cp=zListCellNext(cp) ){
    nv = zShape3DContigVert( zShapeListCellShape(cp), p, d );
    if( *d < dmin ){
      v = nv;
      dmin = *d;
    }
  }
  *d = dmin;
  return v;
}

/* the closest point to a shape in a list. */
double zShapeListClosest(const zShapeList *list, const zVec3D *p, zVec3D *cp)
{
  zShapeListCell *c;
  zVec3D ncp;
  double d, dmin;

  c = zListTail( list );
  dmin = zShape3DClosest( zShapeListCellShape(c), p, cp );
  for( c=zListCellNext(c); c!=zListRoot(list); c=zListCellNext(c) )
    if( ( d = zShape3DClosest( zShapeListCellShape(c), p, &ncp ) ) < dmin ){
      zVec3DCopy( &ncp, cp );
      dmin = d;
    }
  return dmin;
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
