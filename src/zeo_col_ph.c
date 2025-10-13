/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_ph - collision checking: polyhedron.
 */

#include <zeo/zeo_col.h>

/* check if two polyhedra intersect with each other. */
bool zColChkPH3D(zPH3D *ph1, zPH3D *ph2, zVec3D *p1, zVec3D *p2)
{
  zVec3D _p1, _p2;
  zVec3DData data1, data2;

  if( p1 == NULL ) p1 = &_p1;
  if( p2 == NULL ) p2 = &_p2;
  zVec3DDataAssignArray( &data1, zPH3DVertArray(ph1) );
  zVec3DDataAssignArray( &data2, zPH3DVertArray(ph2) );
  return zGJK( &data1, &data2, p1, p2 );
}

/* Muller-Preparata's algorithm */

#define ZEO_TRI3D_DUALXFORM_MIN_OFFSET ( 1.0e-5 )
/* dual transform from/to point to/from plane. */
static zVec3D *_zTri3DDualXform(zTri3D *t, zVec3D *p, zVec3D *dx)
{
  double d;

  d = zVec3DInnerProd(zTri3DNorm(t),p);
  /* replace distance between the given triangle and the base point with a predefined value if it is too short. */
  if( d < 0 && d > -ZEO_TRI3D_DUALXFORM_MIN_OFFSET ) d =-ZEO_TRI3D_DUALXFORM_MIN_OFFSET;
  else
  if( d >= 0 && d < ZEO_TRI3D_DUALXFORM_MIN_OFFSET ) d = ZEO_TRI3D_DUALXFORM_MIN_OFFSET;
  return zVec3DDiv( zTri3DNorm(t), d, dx );
}

/* dual transform from/to point to/from plane shifting origin. */
static zVec3D *_zTri3DDualXform_a(zTri3D *t, zVec3D *c, zVec3D *dx)
{
  zVec3D p;

  zVec3DSub( zTri3DVert(t,0), c, &p );
  return _zTri3DDualXform( t, &p, dx );
}

/* simple dual transform from/to point to/from plane. */
static zVec3D *_zTri3DDualXform_b(zTri3D *t, zVec3D *dx)
{
  return _zTri3DDualXform( t, zTri3DVert(t,0), dx );
}

/* return a face of a polyhedron close to a point. */
static zTri3D *_zIntersectPH3DPointIsInside(zPH3D *ph, zVec3D *p, double *dis)
{
  int i;

  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    *dis = zTri3DDistFromPointToPlane( zPH3DFace(ph,i), p );
    if( *dis >= -zTOL ){
      *dis += zTOL;
      return zPH3DFace(ph,i);
    }
  }
  return NULL;
}

#define Z_INTERSECT_PH3D_FAILURE( label ) do{ \
  phcol = NULL; \
  goto label; \
} while(0)

/* intersection of convices by Muller-Preparata's algorithm. */
zPH3D *zIntersectPH3D(zPH3D *ph1, zPH3D *ph2, zPH3D *phcol)
{
  zVec3DData data1, data2;
  zVec3D p1, p2, p_temp;
  zPH3D ch;
  int i, n;
  double dis1, dis2;
  zTri3D *tri1, *tri2;

  zPH3DInit( phcol );
  zVec3DDataAssignArray( &data1, zPH3DVertArray(ph1) );
  zVec3DDataAssignArray( &data2, zPH3DVertArray(ph2) );
  /* the base point with an ad-hoc modulation */
  if( !zGJKDepth( &data1, &data2, &p1, &p2 ) ) return NULL;
  zVec3DMid( &p1, &p2, &p_temp );
  zVec3DCopy( &p_temp, &p1 );
  for( dis1=dis2=0, i=0; i<=Z_MAX_ITER_NUM; i++ ){
    tri1 = _zIntersectPH3DPointIsInside( ph1, &p1, &dis1 );
    tri2 = _zIntersectPH3DPointIsInside( ph2, &p1, &dis2 );
    if( !tri1 && !tri2 ) break;
    if( i == Z_MAX_ITER_NUM ) return NULL;
    if( tri1 ) zVec3DCatDRC( &p1, -zTOL-dis1, zTri3DNorm(tri1) );
    if( tri2 ) zVec3DCatDRC( &p1, -zTOL-dis2, zTri3DNorm(tri2) );
  }
  zVec3DDataDestroy( &data1 );
  zVec3DDataDestroy( &data2 );

  /* transfer to dual space */
  if( !zVec3DDataInitArray( &data1, zPH3DFaceNum(ph1) + zPH3DFaceNum(ph2) ) )
    return NULL;
  /* dual-transfer triangles intersecting with the roughly-estimated intersection volume */
  n = 0;
  for( i=0; i<zPH3DFaceNum(ph1); i++ ){
    if( !_zTri3DDualXform_a( zPH3DFace(ph1,i), &p1, &p_temp ) || !zVec3DDataAdd( &data1, &p_temp ) )
      Z_INTERSECT_PH3D_FAILURE( TERMINATE );
    n++;
  }
  for( i=0; i<zPH3DFaceNum(ph2); i++ ){
    if( !_zTri3DDualXform_a( zPH3DFace(ph2,i), &p1, &p_temp ) || !zVec3DDataAdd( &data1, &p_temp ) )
      Z_INTERSECT_PH3D_FAILURE( TERMINATE );
    n++;
  }
  /* convex hull in dual space */
  if( !zVec3DDataConvexHull( &data1, &ch ) ) Z_INTERSECT_PH3D_FAILURE( TERMINATE );
  zVec3DDataDestroy( &data1 );
  /* re-transfer to the original space */
  if( !zVec3DDataInitArray( &data1, zPH3DFaceNum(&ch) ) ) Z_INTERSECT_PH3D_FAILURE( TERMINATE );
  for( i=0; i<zPH3DFaceNum(&ch); i++ ){
    if( !_zTri3DDualXform_b( zPH3DFace(&ch,i), &p_temp ) ) Z_INTERSECT_PH3D_FAILURE( TERMINATE );
    zVec3DAddDRC( &p_temp, &p1 );
    zVec3DDataAdd( &data1, &p_temp );
  }
  if( !zVec3DDataConvexHull( &data1, phcol ) ) phcol = NULL;

 TERMINATE:
  zPH3DDestroy( &ch );
  zVec3DDataDestroy( &data1 );
  return phcol;
}
