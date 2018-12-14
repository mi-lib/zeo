/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_list - list of 3D vectors.
 */

#include <zeo/zeo_vec3d.h>

/* ********************************************************** */
/* CLASS: zVec3DList
 * list of 3D vectors
 * ********************************************************** */

/* zVec3DListInsert
 * - insert a 3D vector to a vector list.
 */
zVec3DListCell *zVec3DListInsert(zVec3DList *list, zVec3D *v)
{
  zVec3DListCell *cell;

  if( !( cell = zAlloc( zVec3DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( cell->data = zAlloc( zVec3D, 1 ) ) ){
    ZALLOCERROR();
    free( cell );
    return NULL;
  }
  zVec3DCopy( v, cell->data );
  zListInsertHead( list, cell );
  return cell;
}

/* zVec3DListFromArray
 * - create a list of vectors from an array of 3D vectors.
 */
zVec3DList *zVec3DListFromArray(zVec3DList *list, zVec3D v[], int num)
{
  zListInit( list );
  while( num-- > 0 )
    if( !zVec3DListInsert( list, v++ ) ){
      ZALLOCERROR();
      break;
    }
  return list;
}

void zVec3DListDestroy(zVec3DList *list)
{
  zVec3DListCell *vc;

  while( !zListIsEmpty( list ) ){
    zListDeleteHead( list, &vc );
    zFree( vc->data );
    zFree( vc );
  }
}

/* zVec3DListQuickSort
 * - a quick sort routine for vector list class.
 */
zListQuickSortDef( zVec3DList, zVec3DListCell )

/* zVec3DListFWrite
 * - output a list of 3D vectors.
 */
void zVec3DListFWrite(FILE *fp, zVec3DList *list)
{
  zVec3DListCell *cp;

  fprintf( fp, "%d\n", zListNum(list) );
  zListForEach( list, cp )
    zVec3DFWrite( fp, cp->data );
}

/* zVec3DListDataFWrite
 * - output a list of 3D vectors in a plain form.
 */
void zVec3DListDataFWrite(FILE *fp, zVec3DList *list)
{
  zVec3DListCell *cp;

  zListForEach( list, cp )
    zVec3DDataNLFWrite( fp, cp->data );
}

/* ********************************************************** */
/* CLASS: zVec3DAddrList
 * list of pointers to 3D vectors
 * ********************************************************** */

/* zVec3DAddrListInsert
 * - insert a pointer to a 3D vector into a list of vectors.
 */
zVec3DAddr *zVec3DAddrListInsert(zVec3DAddrList *list, zVec3D *v)
{
  zVec3DAddr *cell;

  if( !( cell = zAlloc( zVec3DAddr, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  cell->data = v;
  zListInsertHead( list, cell );
  return cell;
}

/* zVec3DAddrListFromArray
 * - create a list of vectors from an array of 3D vectors.
 */
zVec3DAddrList *zVec3DAddrListFromArray(zVec3DAddrList *list, zVec3D v[], int num)
{
  zListInit( list );
  while( num-- > 0 )
    if( !zVec3DAddrListInsert( list, v++ ) ){
      ZALLOCERROR();
      break;
    }
  return list;
}

/* zVec3DAddrListQuickSort
 * - a quick sort routine for vector list class.
 */
zListQuickSortDef( zVec3DAddrList, zVec3DAddr )

/* zVec3DAddrListFWrite
 * - output a list of pointers to 3D vectors.
 */
void zVec3DAddrListFWrite(FILE *fp, zVec3DAddrList *list)
{
  zVec3DAddr *cp;

  fprintf( fp, "%d\n", zListNum(list) );
  zListForEach( list, cp )
    zVec3DFWrite( fp, cp->data );
}

/* zVec3DAddrListDataFWrite
 * - output a list of pointers to 3D vectors in a plain form.
 */
void zVec3DAddrListDataFWrite(FILE *fp, zVec3DAddrList *list)
{
  zVec3DAddr *cp;

  zListForEach( list, cp )
    zVec3DDataNLFWrite( fp, cp->data );
}

/* ********************************************************** */
/* utilities for point clouds
 * ********************************************************** */

/* zVec3DListNN
 * - a naive algorithm to find the nearest neighbor in a list of 3D vectors.
 */
zVec3D *zVec3DListNN(zVec3DList *list, zVec3D *v, double *dmin)
{
  zVec3DListCell *cell;
  double d;
  zVec3D *nn = NULL;

  *dmin = HUGE_VAL;
  zListForEach( list, cell )
    if( ( d = zVec3DSqrDist( cell->data, v ) ) < *dmin ){
      *dmin = d;
      nn = cell->data;
    }
  *dmin = sqrt( *dmin );
  return nn;
}
