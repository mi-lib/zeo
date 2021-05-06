/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d - 2D element.
 */

#include <zeo/zeo_elem2d.h>

/* ********************************************************** */
/* CLASS: zTri2D
 * 2D triangle class
 * ********************************************************** */

/* create a triangle. */
zTri2D *zTri2DCreate(zTri2D *t, zVec2D *v1, zVec2D *v2, zVec2D *v3)
{
  zTri2DSetVert( t, 0, v1 );
  zTri2DSetVert( t, 1, v2 );
  zTri2DSetVert( t, 2, v3 );
  return t;
}

/* barycenter of a triangle. */
zVec2D *zTri2DBarycenter(zTri2D *t, zVec2D *c)
{
  c->c.x = ( zTri2DVert(t,0)->c.x + zTri2DVert(t,1)->c.x + zTri2DVert(t,2)->c.x ) / 3;
  c->c.y = ( zTri2DVert(t,0)->c.y + zTri2DVert(t,1)->c.y + zTri2DVert(t,2)->c.y ) / 3;
  return c;
}

/* circumcenter of a 2D triangle */
static double __z_tri2D_angle(zTri2D *t, int i, int j, int k){
  zVec2D e[2];
  zVec2DSub( zTri2DVert(t,j), zTri2DVert(t,i), &e[0] );
  zVec2DSub( zTri2DVert(t,k), zTri2DVert(t,i), &e[1] );
  return sin( 2 * zVec2DAngle( &e[0], &e[1] ) );
}
zVec2D *zTri2DCircumcenter(zTri2D *t, zVec2D *c)
{
  double s[3];

  s[0] = __z_tri2D_angle( t, 0, 1, 2 );
  s[1] = __z_tri2D_angle( t, 1, 2, 0 );
  s[2] = __z_tri2D_angle( t, 2, 0, 1 );
  zVec2DZero( c );
  zVec2DCatDRC( c, s[0], zTri2DVert(t,0) );
  zVec2DCatDRC( c, s[1], zTri2DVert(t,1) );
  zVec2DCatDRC( c, s[2], zTri2DVert(t,2) );
  return zVec2DDivDRC( c, s[0] + s[1] + s[2] );
}

/* incenter of a triangle */
zVec2D *zTri2DIncenter(zTri2D *t, zVec2D *c)
{
  double s[3];

  s[0] = zVec2DDist( zTri2DVert(t,1), zTri2DVert(t,2) );
  s[1] = zVec2DDist( zTri2DVert(t,2), zTri2DVert(t,0) );
  s[2] = zVec2DDist( zTri2DVert(t,0), zTri2DVert(t,1) );
  zVec2DZero( c );
  zVec2DCatDRC( c, s[0], zTri2DVert(t,0) );
  zVec2DCatDRC( c, s[1], zTri2DVert(t,1) );
  zVec2DCatDRC( c, s[2], zTri2DVert(t,2) );
  return zVec2DDivDRC( c, s[0] + s[1] + s[2] );
}

/* orthocenter of a triangle */
zVec2D *zTri2DOrthocenter(zTri2D *t, zVec2D *c)
{
  zVec2D cg, cc;

  zTri2DBarycenter( t, &cg );
  zTri2DCircumcenter( t, &cc );
  zVec2DMulDRC( &cg, 3 );
  zVec2DMulDRC( &cc, 2 );
  return zVec2DSub( &cg, &cc, c );
}

/* print information of a triangle to a file. */
void zTri2DFPrint(FILE *fp, zTri2D *t)
{
  if( !t )
    fprintf( fp, "(null triangle)\n" );
  else{
    fprintf( fp, "vert: 0 " );
    zVec2DFPrint( fp, zTri2DVert(t,0) );
    fprintf( fp, "vert: 1 " );
    zVec2DFPrint( fp, zTri2DVert(t,1) );
    fprintf( fp, "vert: 2 " );
    zVec2DFPrint( fp, zTri2DVert(t,2) );
  }
}

/* ********************************************************** */
/* CLASS: zDisk2D
 * 2D disk class.
 * ********************************************************** */

/* create a 2D disk. */
zDisk2D *zDisk2DCreate(zDisk2D *d, zVec2D *c, double r)
{
  zDisk2DSetCenter( d, c );
  zDisk2DSetRadius( d, r );
  return d;
}

/* signed distance from a 2D point to a disk. */
double zDisk2DPointDist(zDisk2D *d, zVec2D *p)
{
  return zVec2DDist( zDisk2DCenter(d), p ) - zDisk2DRadius(d);
}

/* check if a 2D point is inside of a disk. */
bool zDisk2DPointIsInside(zDisk2D *d, zVec2D *p, bool rim)
{
  return zDisk2DPointDist( d, p ) < ( rim ? zTOL : 0 ) ? true : false;
}

/* create a 2D disk from two points at both ends of diameter. */
zDisk2D *zDisk2DFrom2(zDisk2D *d, zVec2D *v1, zVec2D *v2)
{
  zVec2DMid( v1, v2, zDisk2DCenter(d) );
  zDisk2DSetRadius( d, zVec2DDist(zDisk2DCenter(d),v1) );
  return d;
}

/* create a 2D disk from three points as the circumcircle of them. */
zDisk2D *zDisk2DFrom3(zDisk2D *d, zVec2D *v1, zVec2D *v2, zVec2D *v3)
{
  zTri2D t;

  zTri2DCreate( &t, v1, v2, v3 );
  zTri2DCircumcenter( &t, zDisk2DCenter(d) );
  zDisk2DSetRadius( d, zVec2DDist(zDisk2DCenter(d),v3) );
  return d;
}
