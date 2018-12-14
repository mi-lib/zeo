/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem_list - 3D shape element list.
 */

#include <zeo/zeo_elem.h>

/* ********************************************************** */
/* CLASS: zTri3DList
 * 3D triangle list
 * ********************************************************** */

/* zTri3DListInsert
 * - insert 3D triangle list cell.
 */
zTri3DListCell *zTri3DListInsert(zTri3DList *list, zTri3D *t)
{
  zTri3DListCell *cell;

  if( !( cell = zAlloc( zTri3DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zCopy( zTri3D, t, &cell->data );
  zListInsertHead( list, cell );
  return cell;
}

/* zTri3DListAlign
 * - align triangles to a direction referred by a vector.
 */
void zTri3DListAlign(zTri3DList *list, zVec3D *ref)
{
  zTri3DListCell *tp;

  zListForEach( list, tp )
    if( zVec3DInnerProd( ref, zTri3DNorm(&tp->data) ) < 0 )
      zTri3DRevDRC( &tp->data );
}

/* zTri3DListCopyArray
 * - copy triangles in a list to array.
 */
void zTri3DListCopyArray(zTri3DList *list, zTri3D t[], int n)
{
  zTri3DListCell *tp;
  register int i = 0;

  zListForEach( list, tp ){
    zCopy( zTri3D, &tp->data, &t[i] );
    if( ++i >= n ) break;
  }
}
