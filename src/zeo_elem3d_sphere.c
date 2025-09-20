/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d_sphere - 3D sphere.
 */

#include <zeo/zeo_elem3d.h>

/* ********************************************************** */
/* 3D sphere class
 * ********************************************************** */

/* create a 3D sphere. */
zSphere3D *zSphere3DCreate(zSphere3D *sphere, const zVec3D *c, double r, int div)
{
  zSphere3DSetCenter( sphere, c );
  zSphere3DSetRadius( sphere, r );
  zSphere3DSetDiv( sphere, div == 0 ? ZEO_ELEM_DEFAULT_DIV : div );
  return sphere;
}

/* initialize a 3D sphere. */
zSphere3D *zSphere3DInit(zSphere3D *sphere)
{
  return zSphere3DCreate( sphere, ZVEC3DZERO, 0, 0 );
}

/* allocate memory for a 3D sphere. */
ZDEF_ALLOC_FUNCTION( zSphere3D )

/* copy a 3D sphere. */
zSphere3D *zSphere3DCopy(const zSphere3D *src, zSphere3D *dest)
{
  return zSphere3DCreate( dest,
    zSphere3DCenter(src), zSphere3DRadius(src), zSphere3DDiv(src) );
}

/* mirror a 3D sphere. */
zSphere3D *zSphere3DMirror(const zSphere3D *src, zSphere3D *dest, zAxis axis)
{
  zSphere3DCopy( src, dest );
  zSphere3DCenter(dest)->e[(int)axis] *= -1;
  return dest;
}

/* transform coordinates of a 3D sphere. */
zSphere3D *zSphere3DXform(const zSphere3D *src, const zFrame3D *f, zSphere3D *dest)
{
  zXform3D( f, zSphere3DCenter(src), zSphere3DCenter(dest) );
  zSphere3DSetRadius( dest, zSphere3DRadius(src) );
  zSphere3DSetDiv( dest, zSphere3DDiv(src) );
  return dest;
}

/* inversely transform coordinates of a 3D sphere. */
zSphere3D *zSphere3DXformInv(const zSphere3D *src, const zFrame3D *f, zSphere3D *dest)
{
  zXform3DInv( f, zSphere3DCenter(src), zSphere3DCenter(dest) );
  zSphere3DSetRadius( dest, zSphere3DRadius(src) );
  zSphere3DSetDiv( dest, zSphere3DDiv(src) );
  return dest;
}

/* closest point from a point to a sphere. */
double zSphere3DClosest(const zSphere3D *sphere, const zVec3D *p, zVec3D *cp)
{
  zVec3D d;
  double l;

  zVec3DSub( p, zSphere3DCenter(sphere), &d );
  if( ( l = zVec3DNorm(&d) ) < zSphere3DRadius(sphere) )
    zVec3DCopy( p, cp );
  else{
    zVec3DMulDRC( &d, zSphere3DRadius(sphere)/l );
    zVec3DAdd( zSphere3DCenter(sphere), &d, cp );
  }
  return l - zSphere3DRadius(sphere);
}

/* distance between a 3D sphere and a point. */
double zSphere3DDistFromPoint(const zSphere3D *sphere, const zVec3D *p)
{
  zVec3D d;

  zVec3DSub( p, zSphere3DCenter(sphere), &d );
  return zVec3DNorm(&d) - zSphere3DRadius(sphere);
}

/* judge if a point is inside of a 3D sphere. */
bool zSphere3DPointIsInside(const zSphere3D *sphere, const zVec3D *p, double margin)
{
  return zSphere3DDistFromPoint( sphere, p ) < margin ? true : false;
}

/* create a 3D sphere from two points at both ends of diameter. */
zSphere3D *zSphere3DFrom2(zSphere3D *sphere, const zVec3D *v1, const zVec3D *v2)
{
  zVec3D c;
  double r;

  r = zVec3DDist( zVec3DMid( v1, v2, &c ), v1 );
  return zSphere3DCreate( sphere, &c, r, 0 );
}

/* create a 3D sphere from three points to include their circumcircle as the great circle. */
zSphere3D *zSphere3DFrom3(zSphere3D *sphere, const zVec3D *v1, const zVec3D *v2, const zVec3D *v3)
{
  zVec3D c;
  zTri3D t;
  double r;

  zTri3DCreate( &t, v1, v2, v3 );
  r = zVec3DDist( zTri3DCircumcenter( &t, &c ), v3 );
  return zSphere3DCreate( sphere, &c, r, 0 );
}

/* create a 3D sphere from four points as the circumscribing sphere of them. */
zSphere3D *zSphere3DFrom4(zSphere3D *sphere, const zVec3D *v1, const zVec3D *v2, const zVec3D *v3, const zVec3D *v4)
{
  zVec3D e1, e2, e3, c;
  zTri3D t;
  double r;

  zVec3DSub( v2, v1, &e1 );
  zVec3DSub( v3, v1, &e2 );
  zVec3DSub( v4, v1, &e3 );
  zTri3DCreate( &t, &e1, &e2, &e3 );
  r = zVec3DNorm( zTri3DConeCircumcenter( &t, &c ) );
  zVec3DAddDRC( &c, v1 );
  return zSphere3DCreate( sphere, &c, r, 0 );
}

/* volume of a 3D sphere. */
double zSphere3DVolume(const zSphere3D *sphere)
{
  return 4.0 * zPI * pow(zSphere3DRadius(sphere),3) / 3.0;
}

/* inertia tensor about barycenter of a 3D sphere from mass. */
zMat3D *zSphere3DBaryInertiaMass(const zSphere3D *sphere, double mass, zMat3D *inertia)
{
  double i;

  i = 0.4 * mass * zSqr( zSphere3DRadius(sphere) );
  zMat3DCreate( inertia, i, 0, 0, 0, i, 0, 0, 0, i );
  return inertia;
}

/* inertia tensor about barycenter of a 3D sphere. */
zMat3D *zSphere3DBaryInertia(const zSphere3D *sphere, double density, zMat3D *inertia)
{
  return zSphere3DBaryInertiaMass( sphere, density * zSphere3DVolume( sphere ), inertia );
}
