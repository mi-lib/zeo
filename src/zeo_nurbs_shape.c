/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs_shape - conversion from 3D shape to NURBS surface.
 */

#include <zeo/zeo_nurbs_shape.h>

/* initialize knots for elliptic intersection. */
static bool _zNURBS3DEllipsInitKnot(zNURBS3D *nurbs, int i)
{
  if( zNURBS3DKnotNum(nurbs,i) != 12 ){
    ZRUNERROR( ZEO_ERR_NURBS_ELLIPS_INVALID_KNOT_NUM, zNURBS3DKnotNum(nurbs,0) );
    return false;
  }
  zNURBS3DSetKnot( nurbs, i, 0, 0.0 );
  zNURBS3DSetKnot( nurbs, i, 1, 0.0 );
  zNURBS3DSetKnot( nurbs, i, 2, 0.0 );
  zNURBS3DSetKnot( nurbs, i, 3, 0.25 );
  zNURBS3DSetKnot( nurbs, i, 4, 0.25 );
  zNURBS3DSetKnot( nurbs, i, 5, 0.5 );
  zNURBS3DSetKnot( nurbs, i, 6, 0.5 );
  zNURBS3DSetKnot( nurbs, i, 7, 0.75 );
  zNURBS3DSetKnot( nurbs, i, 8, 0.75 );
  zNURBS3DSetKnot( nurbs, i, 9, 1.0 );
  zNURBS3DSetKnot( nurbs, i,10, 1.0 );
  zNURBS3DSetKnot( nurbs, i,11, 1.0 );
  return true;
}

/* initialize control points for elliptic intersection. */
static bool _zNURBS3DEllipsInitCP(zNURBS3D *nurbs, int i, const zVec3D *center, double rx, double ry, const zVec3D *ax, const zVec3D *ay)
{
  if( zNURBS3DCPNum(nurbs,0) != 9 ){
    ZRUNERROR( ZEO_ERR_NURBS_ELLIPS_INVALID_CP_NUM, zNURBS3DCPNum(nurbs,0) );
    return false;
  }
  zVec3DCat( center, rx, ax, zNURBS3DCP(nurbs,0,i) );
  zVec3DCat( zNURBS3DCP(nurbs,0,i), ry, ay, zNURBS3DCP(nurbs,1,i) );
  zVec3DCat( center, ry, ay, zNURBS3DCP(nurbs,2,i) );
  zVec3DCat( zNURBS3DCP(nurbs,2,i),-rx, ax, zNURBS3DCP(nurbs,3,i) );
  zVec3DCat( center,-rx, ax, zNURBS3DCP(nurbs,4,i) );
  zVec3DCat( zNURBS3DCP(nurbs,4,i),-ry, ay, zNURBS3DCP(nurbs,5,i) );
  zVec3DCat( center,-ry, ay, zNURBS3DCP(nurbs,6,i) );
  zVec3DCat( zNURBS3DCP(nurbs,6,i), rx, ax, zNURBS3DCP(nurbs,7,i) );
  zVec3DCopy( zNURBS3DCP(nurbs,0,i), zNURBS3DCP(nurbs,8,i) );
  return true;
}

/* closed NURBS surface that represents an elliptic cylinder. */
zNURBS3D *zNURBS3DECyl(zNURBS3D *nurbs, const zECyl3D *ecyl)
{
  zVec3D axis;
  int i;

  if( !zNURBS3DAlloc( nurbs, 9, 2, 2, 1 ) ) return NULL;
  _zNURBS3DEllipsInitCP( nurbs, 0, zECyl3DCenter(ecyl,0), zECyl3DRadius(ecyl,0), zECyl3DRadius(ecyl,1), zECyl3DRadVec(ecyl,0), zECyl3DRadVec(ecyl,1) );
  zECyl3DAxis( ecyl, &axis );
  for( i=0; i<9; i++ ){
    zVec3DAdd( zNURBS3DCP(nurbs,i,0), &axis, zNURBS3DCP(nurbs,i,1) );
    zNURBS3DSetWeight( nurbs, i, 0, zIsEven(i) ? 1.0 : 1.0/sqrt(2) );
    zNURBS3DSetWeight( nurbs, i, 1, zNURBS3DWeight(nurbs,i,0) );
  }
  _zNURBS3DEllipsInitKnot( nurbs, 0 );
  zNURBS3DSetKnot( nurbs, 1, 0, 0.0 );
  zNURBS3DSetKnot( nurbs, 1, 1, 0.0 );
  zNURBS3DSetKnot( nurbs, 1, 2, 1.0 );
  zNURBS3DSetKnot( nurbs, 1, 3, 1.0 );
  return nurbs;
}

/* closed NURBS surface that represents a cone. */
zNURBS3D *zNURBS3DCone(zNURBS3D *nurbs, const zCone3D *cone)
{
  zVec3D axis;
  zVec3D ax, ay;
  int i;

  if( !zNURBS3DAlloc( nurbs, 9, 2, 2, 1 ) ) return NULL;
  zCone3DAxis( cone, &axis );
  zVec3DOrthoSpace( &axis, &ax, &ay );
  zVec3DNormalizeDRC( &ax );
  zVec3DNormalizeDRC( &ay );
  _zNURBS3DEllipsInitCP( nurbs, 0, zCone3DCenter(cone), zCone3DRadius(cone), zCone3DRadius(cone), &ax, &ay );
  for( i=0; i<9; i++ ){
    zVec3DAdd( zCone3DCenter(cone), &axis, zNURBS3DCP(nurbs,i,1) );
    zNURBS3DSetWeight( nurbs, i, 0, zIsEven(i) ? 1.0 : 1.0/sqrt(2) );
    zNURBS3DSetWeight( nurbs, i, 1, zNURBS3DWeight(nurbs,i,0) );
  }
  _zNURBS3DEllipsInitKnot( nurbs, 0 );
  zNURBS3DSetKnot( nurbs, 1, 0, 0.0 );
  zNURBS3DSetKnot( nurbs, 1, 1, 0.0 );
  zNURBS3DSetKnot( nurbs, 1, 2, 1.0 );
  zNURBS3DSetKnot( nurbs, 1, 3, 1.0 );
  return nurbs;
}

/* closed NURBS surface that represents an ellipsoid. */
zNURBS3D *zNURBS3DEllips(zNURBS3D *nurbs, const zEllips3D *ellips)
{
  int i, j;
  double wi;

  if( !zNURBS3DAlloc( nurbs, 9, 5, 2, 2 ) ) return NULL;
  _zNURBS3DEllipsInitCP( nurbs, 2, zEllips3DCenter(ellips), zEllips3DRadiusX(ellips), zEllips3DRadiusY(ellips), zEllips3DAxis(ellips,zX), zEllips3DAxis(ellips,zY) );
  for( i=0; i<9; i++ ){
    zVec3DCat( zEllips3DCenter(ellips),-zEllips3DRadiusZ(ellips), zEllips3DAxis(ellips,zZ), zNURBS3DCP(nurbs,i,0) );
    zVec3DCat( zNURBS3DCP(nurbs,i,2),-zEllips3DRadiusZ(ellips), zEllips3DAxis(ellips,zZ), zNURBS3DCP(nurbs,i,1) );
    zVec3DCat( zNURBS3DCP(nurbs,i,2), zEllips3DRadiusZ(ellips), zEllips3DAxis(ellips,zZ), zNURBS3DCP(nurbs,i,3) );
    zVec3DCat( zEllips3DCenter(ellips), zEllips3DRadiusZ(ellips), zEllips3DAxis(ellips,zZ), zNURBS3DCP(nurbs,i,4) );
    wi = zIsEven(i) ? 1.0 : 1.0/sqrt(2);
    for( j=0; j<5; j++ ){
      zNURBS3DSetWeight( nurbs, i, j, wi * ( zIsEven(j) ? 1.0 : 1.0/sqrt(2) ) );
    }
  }
  _zNURBS3DEllipsInitKnot( nurbs, 0 );
  zNURBS3DSetKnot( nurbs, 1, 0, 0.0 );
  zNURBS3DSetKnot( nurbs, 1, 1, 0.0 );
  zNURBS3DSetKnot( nurbs, 1, 2, 0.0 );
  zNURBS3DSetKnot( nurbs, 1, 3, 0.5 );
  zNURBS3DSetKnot( nurbs, 1, 4, 0.5 );
  zNURBS3DSetKnot( nurbs, 1, 5, 1.0 );
  zNURBS3DSetKnot( nurbs, 1, 6, 1.0 );
  zNURBS3DSetKnot( nurbs, 1, 7, 1.0 );
  return nurbs;
}

/* closed NURBS surface that represents a torus. */
zNURBS3D *zNURBS3DTorus(zNURBS3D *nurbs, const zVec3D *center, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double rring, double rintersection)
{
  zVec3D p;
  double r, b;
  double wj;
  int i, j;

  if( !zNURBS3DAlloc( nurbs, 9, 9, 2, 2 ) ) return NULL;
  for( j=0; j<9; j++ ){
    switch( j ){
    case 2: case 6:                 r = rring;      break;
    case 0: case 1: case 7: case 8: r = rring + rintersection; break;
    case 3: case 4: case 5:         r = rring - rintersection; break;
    }
    switch( j ){
    case 0: case 4: case 8: b = 0;  break;
    case 1: case 2: case 3: b = rintersection; break;
    case 5: case 6: case 7: b =-rintersection; break;
    }
    zVec3DCat( center, b, az, &p );
    _zNURBS3DEllipsInitCP( nurbs, j, &p, r, r, ax, ay );
    wj = zIsEven(j) ? 1.0 : 1.0/sqrt(2);
    for( i=0; i<9; i++ ){
      zNURBS3DSetWeight( nurbs, i, j, ( zIsEven(i) ? 1.0 : 1.0/sqrt(2) ) * wj );
    }
  }
  _zNURBS3DEllipsInitKnot( nurbs, 0 );
  _zNURBS3DEllipsInitKnot( nurbs, 1 );
  return nurbs;
}
