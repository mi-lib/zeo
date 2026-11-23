/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_obb - 3D bounding volume: oriented bounding box.
 */

#include <zeo/zeo_bv2d.h>
#include <zeo/zeo_bv3d.h>

/* The oriented bounding box is computed as follows:
 * 1. the z-axis is the normal vector of the face of the convex hull, along which the extent of the
 *    convex hull is the minimum,
 * 2. the x- and y-axes are those of the minimum-area rectangle that encloses the projection of the
 *    convex hull onto the plane orthogonal to the z-axis, which is found by the rotating calipers
 *    for the convex hull of the projected vertices.
 * Note that it is not the minimum-volume box in general. */

/* minimum extent of a convex hull along the normal vectors of the faces, where faces with the zero
 * normal vector (degenerate faces) are skipped. The z-axis of the box is set for the normal vector,
 * and the lowest and highest values of the vertices along it are put into zmin and zmax. */
static bool _zOBB3DMinDir(zBox3D *obb, zPH3D *ch, double *zmin, double *zmax)
{
  zTri3D *bot = NULL;
  double l, h, hmin = HUGE_VAL;
  int i, j;

  for( i=0; i<zPH3DFaceNum(ch); i++ ){
    if( zVec3DIsTiny( zTri3DNorm(zPH3DFace(ch,i)) ) ) continue; /* degenerate face */
    for( h=0, j=0; j<zPH3DVertNum(ch); j++ )
      if( ( l = -zTri3DDistFromPointToPlane( zPH3DFace(ch,i), zPH3DVert(ch,j) ) ) > h ) h = l;
    if( h < hmin ){
      bot = zPH3DFace(ch,i);
      hmin = h;
    }
  }
  if( !bot ) return false;
  zVec3DCopy( zTri3DNorm(bot), zBox3DAxis(obb,zZ) );
  *zmax = zVec3DInnerProd( zTri3DVert(bot,0), zBox3DAxis(obb,zZ) );
  *zmin = *zmax - hmin;
  return true;
}

/* project vertices of a convex hull onto the plane spanned by orthonormal vectors ex and ey. */
static zVec2D *_zOBB3DProj(zPH3D *ch, zVec3D *ex, zVec3D *ey)
{
  zVec2D *p;
  int i;

  if( !( p = zAlloc( zVec2D, zPH3DVertNum(ch) ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  for( i=0; i<zPH3DVertNum(ch); i++ )
    zVec2DCreate( &p[i], zVec3DInnerProd( zPH3DVert(ch,i), ex ), zVec3DInnerProd( zPH3DVert(ch,i), ey ) );
  return p;
}

/* minimum-area rectangle that encloses a convex polygon of n vertices v[] in counterclockwise order
 * by the rotating calipers, where one side of the rectangle is on an edge of the polygon. The unit
 * vector along the side, the center and the lengths of the sides along and across it are put into
 * e, c, w and h, respectively. */
static void _zOBB3DMinRect(zVec2D v[], int n, zVec2D *e, zVec2D *c, double *w, double *h)
{
  zVec2D ei, ni, d;
  double area, area_min = HUGE_VAL, s0, s1, t;
  int i, j = 1, k = 0, l = 0;

  zVec2DCreate( e, 1, 0 );
  zVec2DZero( c );
  *w = *h = 0;
  for( i=0; i<n; i++ ){
    /* unit vector along the i-th edge, and the normal vector to it directing inward. The edge is not
       zero, but can be very short. The direction is still valid, since the difference of the vertices
       is computed exactly if they are close to each other. */
    zVec2DSub( &v[(i+1)%n], &v[i], &ei );
    zVec2DDivDRC( &ei, zVec2DNorm( &ei ) );
    zVec2DCreate( &ni, -ei.c.y, ei.c.x );
    /* the furthest vertex along ei, that along ni and that against ei, which move monotonically */
    while( zVec2DInnerProd( zVec2DSub( &v[(j+1)%n], &v[j%n], &d ), &ei ) > 0 ) j++;
    if( i == 0 ) k = j;
    while( zVec2DInnerProd( zVec2DSub( &v[(k+1)%n], &v[k%n], &d ), &ni ) > 0 ) k++;
    if( i == 0 ) l = k;
    while( zVec2DInnerProd( zVec2DSub( &v[(l+1)%n], &v[l%n], &d ), &ei ) < 0 ) l++;
    s0 = zVec2DInnerProd( &v[l%n], &ei );
    s1 = zVec2DInnerProd( &v[j%n], &ei );
    t = zVec2DInnerProd( zVec2DSub( &v[k%n], &v[i], &d ), &ni );
    if( ( area = ( s1 - s0 ) * t ) < area_min ){
      area_min = area;
      zVec2DCopy( &ei, e );
      *w = s1 - s0;
      *h = t;
      /* the middle along ei, and the middle between the edge and the furthest vertex along ni */
      zVec2DMul( &ei, 0.5*( s0 + s1 ), c );
      zVec2DCatDRC( c, zVec2DInnerProd( &v[i], &ni ) + 0.5*t, &ni );
    }
  }
}

/* oriented bounding box of 3D points. */
zBox3D *zVec3DDataOBB(zVec3DData *data, zBox3D *obb)
{
  zPH3D ch;
  zVec3D ex, ey, ez;
  zVec2DData data2d;
  zPG2D ch2d, *ch2d_ret;
  zVec2D *p = NULL, e, c;
  double zmin, zmax, w, h;
  zBox3D *ret = NULL;

  if( !zVec3DDataConvexHull( data, &ch ) ) return NULL;
  zPG2DInit( &ch2d );
  if( !_zOBB3DMinDir( obb, &ch, &zmin, &zmax ) ) goto TERMINATE;
  /* orthonormal frame on the plane orthogonal to the z-axis */
  zVec3DCopy( zBox3DAxis(obb,zZ), &ez );
  if( !zVec3DOrthonormalSpace( &ez, &ex, &ey ) ) goto TERMINATE;
  zVec3DOuterProd( &ez, &ex, &ey ); /* to be right-handed */
  /* the minimum-area rectangle on the plane */
  if( !( p = _zOBB3DProj( &ch, &ex, &ey ) ) ||
      !zVec2DDataAssignArrayDirect( &data2d, p, zPH3DVertNum(&ch) ) ) goto TERMINATE;
  ch2d_ret = zVec2DDataConvexHull( &data2d, &ch2d );
  zVec2DDataDestroy( &data2d );
  if( !ch2d_ret ) goto TERMINATE;
  _zOBB3DMinRect( zPG2DVertBuf(&ch2d), zPG2DVertNum(&ch2d), &e, &c, &w, &h );
  /* axes, center and sizes of the box */
  zVec3DMul( &ex, e.c.x, zBox3DAxis(obb,zX) );
  zVec3DCatDRC( zBox3DAxis(obb,zX), e.c.y, &ey );
  zVec3DOuterProd( zBox3DAxis(obb,zZ), zBox3DAxis(obb,zX), zBox3DAxis(obb,zY) );
  zVec3DMul( &ex, c.c.x, zBox3DCenter(obb) );
  zVec3DCatDRC( zBox3DCenter(obb), c.c.y, &ey );
  zVec3DCatDRC( zBox3DCenter(obb), 0.5*( zmin + zmax ), &ez );
  zBox3DSetDepth( obb, w );
  zBox3DSetWidth( obb, h );
  zBox3DSetHeight( obb, zmax - zmin );
  ret = obb;
 TERMINATE:
  zPG2DDestroy( &ch2d );
  zFree( p );
  zPH3DDestroy( &ch );
  return ret;
}
