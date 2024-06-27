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

  if( p1 == NULL ) p1 = &_p1;
  if( p2 == NULL ) p2 = &_p2;
  return zGJK( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2), p1, p2 );
}

/* Muller-Preparata's algorithm */

/* dual transfer from/to point to/from plane. */
static zVec3D *_zTri3DDualXform(zTri3D *t, zVec3D *p, zVec3D *dx)
{
  return zVec3DDiv( zTri3DNorm(t), zVec3DInnerProd(zTri3DNorm(t),p), dx );
}

/* dual transfer from/to point to/from plane shifting origin. */
static zVec3D *_zTri3DDualXform_a(zTri3D *t, zVec3D *c, zVec3D *dx)
{
  zVec3D p;

  zVec3DSub( zTri3DVert(t,0), c, &p );
  return _zTri3DDualXform( t, &p, dx );
}

/* simple dual transfer from/to point to/from plane. */
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

/* intersection of convices by Muller-Preparata's algorithm. */
static zPH3D *_zIntersectPH3D(zPH3D *ph1, zPH3D *ph2, zPH3D *phcol, zAABox3D *ib)
{
  zVec3D *v, p1, p2, p_temp;
  zTri3D *tri;
  zPH3D ch;
  int i, n;
  double dis1, dis2;
  zTri3D *tri1, *tri2;

  zPH3DInit( phcol );
  /* the base point with an ad-hoc modulation */
  if( !zGJKDepth( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2), &p1, &p2 ) ) return NULL;
  zVec3DMid( &p1, &p2, &p_temp );
  zVec3DCopy( &p_temp, &p1) ;
  for( dis1=dis2=0, i=0; i<=Z_MAX_ITER_NUM; i++ ){
    tri1 = _zIntersectPH3DPointIsInside( ph1, &p1, &dis1 );
    tri2 = _zIntersectPH3DPointIsInside( ph2, &p1, &dis2 );
    if( !tri1 && !tri2 ) break;
    if( i == Z_MAX_ITER_NUM ) return NULL;
    if( tri1 ) zVec3DCatDRC( &p1, -zTOL-dis1, zTri3DNorm(tri1) );
    if( tri2 ) zVec3DCatDRC( &p1, -zTOL-dis2, zTri3DNorm(tri2) );
  }

  /* transfer to dual space */
  n = zPH3DFaceNum(ph1) + zPH3DFaceNum(ph2);
  if( !( v = zAlloc( zVec3D, n ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  /* dual-transfer triangles intersecting with
     the roughly-estimated intersection volume */
  n = 0;
  if( ib ){
    for( i=0; i<zPH3DFaceNum(ph1); i++ )
      if( zColChkTriAABox3D( ( tri = zPH3DFace(ph1,i) ), ib ) )
        if(!_zTri3DDualXform_a( tri, &p1, &v[n++] )) return NULL;
    for( i=0; i<zPH3DFaceNum(ph2); i++ )
      if( zColChkTriAABox3D( ( tri = zPH3DFace(ph2,i) ), ib ) )
        if(!_zTri3DDualXform_a( tri, &p1, &v[n++] )) return NULL;
  } else{
    for( i=0; i<zPH3DFaceNum(ph1); i++ )
      if(!_zTri3DDualXform_a( zPH3DFace(ph1,i), &p1, &v[n++] )) return NULL;
    for( i=0; i<zPH3DFaceNum(ph2); i++ )
      if(!_zTri3DDualXform_a( zPH3DFace(ph2,i), &p1, &v[n++] )) return NULL;
  }
  /* convex hull in dual space */
  if( !zConvexHull3D( &ch, v, n ) ){
    phcol = NULL;
    goto TERMINATE;
  }
  zFree( v );
  /* re-transfer to the original space */
  if( !( v = zAlloc( zVec3D, zPH3DFaceNum(&ch) ) ) ){
    ZALLOCERROR();
    goto TERMINATE;
  }
  for( i=0; i<zPH3DFaceNum(&ch); i++ ){
    if(!_zTri3DDualXform_b( zPH3DFace(&ch,i), &v[i] )) return NULL;
    zVec3DAddDRC( &v[i], &p1 );
  }
  if( !zConvexHull3D( phcol, v, zPH3DFaceNum(&ch) ) ) phcol = NULL;

 TERMINATE:
  zPH3DDestroy( &ch );
  zFree( v );
  return phcol;
}

/* intersection of convices by Muller-Preparata's algorithm. */
zPH3D *zIntersectPH3D(zPH3D *ph1, zPH3D *ph2, zPH3D *phcol)
{
  return _zIntersectPH3D( ph1, ph2, phcol, NULL );
}

/* intersection of convices by Muller-Preparata's algorithm with a focusing-acceleration. */
zPH3D *zIntersectPH3DFast(zPH3D *ph1, zPH3D *ph2, zPH3D *phcol)
{
  zAABox3D ib;

  /* rough estimation of intersection volume by an axis-aligned box */
  if( !zIntersectPH3DBox( ph1, ph2, &ib ) ) return NULL;
  return _zIntersectPH3D( ph1, ph2, phcol, &ib );
}
