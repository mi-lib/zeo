/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec2d_list - list of 2D vectors.
 */

#include <zeo/zeo_vec2d.h>

/* ********************************************************** */
/* CLASS: zVec2DList
 * list of 2D vectors
 * ********************************************************** */

/* find an identical 2D vector in a list. */
zVec2DListCell *zVec2DListFind(zVec2DList *list, zVec2D *v)
{
  zVec2DListCell *cp;

  zListForEach( list, cp )
    if( zVec2DEqual( cp->data, v ) ) return cp;
  return NULL;
}

/* add a 2D vector to a vector list. */
zVec2DListCell *zVec2DListAdd(zVec2DList *list, zVec2D *v)
{
  zVec2DListCell *cell;

  if( !( cell = zAlloc( zVec2DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( cell->data = zAlloc( zVec2D, 1 ) ) ){
    ZALLOCERROR();
    free( cell );
    return NULL;
  }
  zVec2DCopy( v, cell->data );
  zListInsertHead( list, cell );
  return cell;
}

/* destroy a list of 2D vectors. */
void zVec2DListDestroy(zVec2DList *list)
{
  zVec2DListCell *vc;

  while( !zListIsEmpty( list ) ){
    zListDeleteHead( list, &vc );
    zFree( vc->data );
    zFree( vc );
  }
}

/* ********************************************************** */
/* CLASS: zVec2DAddrList
 * list of pointers to 2D vectors
 * ********************************************************** */

/* add a pointer to a 2D vector into a list of pointers to 2D vectors. */
zVec2DAddr *zVec2DAddrListAdd(zVec2DAddrList *list, zVec2D *v)
{
  zVec2DAddr *cell;

  if( !( cell = zAlloc( zVec2DAddr, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  cell->data = v;
  zListInsertHead( list, cell );
  return cell;
}

/* create a list of pointers to 2D vectors from an array of 2D vectors. */
zVec2DAddrList *zVec2DAddrListCreate(zVec2DAddrList *list, zVec2D v[], int num)
{
  zListInit( list );
  while( num-- > 0 )
    if( !zVec2DAddrListAdd( list, v++ ) ){
      ZALLOCERROR();
      break;
    }
  return list;
}
