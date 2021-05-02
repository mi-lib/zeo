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

/* find an identical 3D vector in a list. */
zVec3DListCell *zVec3DListFind(zVec3DList *list, zVec3D *v)
{
  zVec3DListCell *cp;

  zListForEach( list, cp )
    if( zVec3DEqual( cp->data, v ) ) return cp;
  return NULL;
}

/* add a 3D vector to a vector list. */
zVec3DListCell *zVec3DListAdd(zVec3DList *list, zVec3D *v)
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

/* append an array of 3D vectors to a list. */
zVec3DList *zVec3DListAppendArray(zVec3DList *list, zVec3DArray *array)
{
  register int i;

  for( i=0; i<zArraySize(array); i++ )
    if( !zVec3DListAdd( list, zArrayElemNC(array,i) ) ){
      ZALLOCERROR();
      break;
    }
  return list;
}

/* converts an array of 3D vectors to a list. */
zVec3DList *zVec3DArray2List(zVec3DArray *array, zVec3DList *list)
{
  zListInit( list );
  return zVec3DListAppendArray( list, array );
}

/* destroy a list of 3D vectors. */
void zVec3DListDestroy(zVec3DList *list)
{
  zVec3DListCell *vc;

  while( !zListIsEmpty( list ) ){
    zListDeleteHead( list, &vc );
    zFree( vc->data );
    zFree( vc );
  }
}

/* quick sort of a list of 3D vectors. */
zListQuickSortDef( zVec3DList, zVec3DListCell )

/* print a list of 3D vectors to a file. */
void zVec3DListFPrint(FILE *fp, zVec3DList *list)
{
  zVec3DListCell *cp;

  fprintf( fp, "%d\n", zListSize(list) );
  zListForEach( list, cp )
    zVec3DFPrint( fp, cp->data );
}

/* print a list of 3D vectors in a plain form to a file. */
void zVec3DListDataFPrint(FILE *fp, zVec3DList *list)
{
  zVec3DListCell *cp;

  zListForEach( list, cp )
    zVec3DDataNLFPrint( fp, cp->data );
}

/* ********************************************************** */
/* CLASS: zVec3DAddrList
 * list of pointers to 3D vectors
 * ********************************************************** */

/* add a pointer to a 3D vector into a list of pointers to 3D vectors. */
zVec3DAddr *zVec3DAddrListAdd(zVec3DAddrList *list, zVec3D *v)
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

/* create a list of pointers to 3D vectors from an array of 3D vectors. */
zVec3DAddrList *zVec3DAddrListCreate(zVec3DAddrList *list, zVec3D v[], int num)
{
  zListInit( list );
  while( num-- > 0 )
    if( !zVec3DAddrListAdd( list, v++ ) ){
      ZALLOCERROR();
      break;
    }
  return list;
}

/* clone a list of pointers to 3D vectors. */
zVec3DAddrList *zVec3DAddrListClone(zVec3DAddrList *src, zVec3DAddrList *dest)
{
  zVec3DAddr *scp, *cp;

  zListInit( dest );
  zListForEach( src, scp ){
    if( !( cp = zAlloc( zVec3DAddr, 1 ) ) ){
      ZALLOCERROR();
      return NULL;
    }
    cp->data = scp->data;
    zListInsertHead( dest, cp );
  }
  return dest;
}

/* quick sort of a list of pointers to 3D vectors. */
zListQuickSortDef( zVec3DAddrList, zVec3DAddr )

/* print a list of pointers to 3D vectors to a file. */
void zVec3DAddrListFPrint(FILE *fp, zVec3DAddrList *list)
{
  zVec3DAddr *cp;

  fprintf( fp, "%d\n", zListSize(list) );
  zListForEach( list, cp )
    zVec3DFPrint( fp, cp->data );
}

/* print a list of pointers to 3D vectors in a plain form to a file. */
void zVec3DAddrListDataFPrint(FILE *fp, zVec3DAddrList *list)
{
  zVec3DAddr *cp;

  zListForEach( list, cp )
    zVec3DDataNLFPrint( fp, cp->data );
}

/* ********************************************************** */
/* utilities for point clouds
 * ********************************************************** */

/* a naive algorithm to find the nearest neighbor in a list of 3D vectors. */
double zVec3DListNN(zVec3DList *list, zVec3D *v, zVec3D **nn)
{
  zVec3DListCell *cell;
  double d, dmin;
  zVec3D nn_tmp, *nnp_tmp;

  if( !nn ){
    nnp_tmp = &nn_tmp;
    nn = &nnp_tmp;
  }
  dmin = HUGE_VAL;
  zListForEach( list, cell )
    if( ( d = zVec3DSqrDist( cell->data, v ) ) < dmin ){
      dmin = d;
      *nn = cell->data;
    }
  return sqrt( dmin );
}

/* support map of a set of points with respect to a direction vector. */
zVec3D *zVec3DSupportMap(zVec3D p[], int n, zVec3D *v)
{
  register int i;
  double d, d_max;
  zVec3D *sp;

  if( n <= 0 ){
    ZRUNWARN( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  sp = &p[0];
  d_max = zVec3DInnerProd( sp, v );
  for( i=1; i<n; i++ )
    if( ( d = zVec3DInnerProd( &p[i], v ) ) > d_max ){
      sp = &p[i];
      d_max = d;
    }
  return sp;
}

/* support map of a set of points with respect to a direction vector. */
zVec3D *zVec3DListSupportMap(zVec3DList *pl, zVec3D *v)
{
  double d, d_max;
  zVec3D *sp;
  zVec3DListCell *cp;

  if( zListIsEmpty(pl) ){
    ZRUNWARN( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  sp = zListTail(pl)->data;
  d_max = zVec3DInnerProd( sp, v );
  zListForEach( pl, cp ){
    if( ( d = zVec3DInnerProd( cp->data, v ) ) > d_max ){
      sp = cp->data;
      d_max = d;
    }
  }
  return sp;
}
