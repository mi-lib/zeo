/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_ellips - primitive 3D shapes: ellipsoid.
 */

#include <zeo/zeo_prim.h>

/* ********************************************************** */
/* CLASS: zEllips3D
 * 3D ellipsoid class
 * ********************************************************** */

static bool _zEllips3DFScan(FILE *fp, void *instance, char *buf, bool *success);

/* create a 3D ellipsoid. */
zEllips3D *zEllips3DCreate(zEllips3D *ellips, zVec3D *c, zVec3D *ax, zVec3D *ay, zVec3D *az, double rx, double ry, double rz, int div)
{
  zEllips3DSetCenter( ellips, c );
  zEllips3DSetAxis( ellips, 0, ax );
  zEllips3DSetAxis( ellips, 1, ay );
  zEllips3DSetAxis( ellips, 2, az );
  zEllips3DSetRadiusX( ellips, fabs(rx) );
  zEllips3DSetRadiusY( ellips, fabs(ry) );
  zEllips3DSetRadiusZ( ellips, fabs(rz) );
  zEllips3DSetDiv( ellips, div == 0 ? ZEO_PRIM_DEFAULT_DIV : div );
  return ellips;
}

/* initialize a 3D ellipsoid. */
zEllips3D *zEllips3DInit(zEllips3D *ellips)
{
  return zEllips3DCreateAlign( ellips, ZVEC3DZERO, 0, 0, 0, 0 );
}

/* copy a 3D ellipsoid to another. */
zEllips3D *zEllips3DCopy(zEllips3D *src, zEllips3D *dest)
{
  return zEllips3DCreate( dest, zEllips3DCenter(src),
    zEllips3DAxis(src,0), zEllips3DAxis(src,1), zEllips3DAxis(src,2),
    zEllips3DRadiusX(src), zEllips3DRadiusY(src), zEllips3DRadiusZ(src),
    zEllips3DDiv(src) );
}

/* mirror a 3D ellipsoid. */
zEllips3D *zEllips3DMirror(zEllips3D *src, zEllips3D *dest, zAxis axis)
{
  zEllips3DCopy( src, dest );
  zEllips3DCenter(dest)->e[axis] *= -1;
  zEllips3DAxis(dest,0)->e[axis] *= -1;
  zEllips3DAxis(dest,1)->e[axis] *= -1;
  zEllips3DAxis(dest,2)->e[axis] *= -1;
  return dest;
}

/* transform coordinates of a 3D ellipsoid. */
zEllips3D *zEllips3DXform(zEllips3D *src, zFrame3D *f, zEllips3D *dest)
{
  zXform3D( f, zEllips3DCenter(src), zEllips3DCenter(dest) );
  zMulMat3DVec3D( zFrame3DAtt(f), zEllips3DAxis(src,zX), zEllips3DAxis(dest,zX) );
  zMulMat3DVec3D( zFrame3DAtt(f), zEllips3DAxis(src,zY), zEllips3DAxis(dest,zY) );
  zMulMat3DVec3D( zFrame3DAtt(f), zEllips3DAxis(src,zZ), zEllips3DAxis(dest,zZ) );
  zEllips3DSetRadiusX( dest, zEllips3DRadiusX(src) );
  zEllips3DSetRadiusY( dest, zEllips3DRadiusY(src) );
  zEllips3DSetRadiusZ( dest, zEllips3DRadiusZ(src) );
  zEllips3DSetDiv( dest, zEllips3DDiv(src) );
  return dest;
}

/* inversely transform coordinates of a 3D ellipsoid. */
zEllips3D *zEllips3DXformInv(zEllips3D *src, zFrame3D *f, zEllips3D *dest)
{
  zXform3DInv( f, zEllips3DCenter(src), zEllips3DCenter(dest) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zEllips3DAxis(src,zX), zEllips3DAxis(dest,zX) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zEllips3DAxis(src,zY), zEllips3DAxis(dest,zY) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zEllips3DAxis(src,zZ), zEllips3DAxis(dest,zZ) );
  zEllips3DSetRadiusX( dest, zEllips3DRadiusX(src) );
  zEllips3DSetRadiusY( dest, zEllips3DRadiusY(src) );
  zEllips3DSetRadiusZ( dest, zEllips3DRadiusZ(src) );
  zEllips3DSetDiv( dest, zEllips3DDiv(src) );
  return dest;
}

/* (static)
 * the closest point from a 3D point to an aligned 3D ellipsoid. */
zVec3D *_zEllips3DClosest(double rx, double ry, double rz, zVec3D *v, zVec3D *cp)
{
  double a, b, c, p, q, r, p2, q2, r2, pqr, l;
  zPex pex;
  zComplex ans[6];
  int i;

  a = zSqr( v->e[zX] / rx );
  b = zSqr( v->e[zY] / ry );
  c = zSqr( v->e[zZ] / rz );
  p = rx * rx; p2 = p * p;
  q = ry * ry; q2 = q * q;
  r = rz * rz; r2 = r * r; pqr = p * q * r;
  if( !( pex = zPexAlloc( 6 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zPexSetCoeff( pex, 6, 1 );
  zPexSetCoeff( pex, 5, 2*(p+q+r) );
  zPexSetCoeff( pex, 4, (1-a)*p2 + (1-c)*r2 + (1-b)*q2 + 4*(p*q+q*r+r*p) );
  zPexSetCoeff( pex, 3, 2*(1-a)*(q+r)*p2 + 2*(1-b)*(r+p)*q2 + 2*(1-c)*(p+q)*r2 + 8*p*q*r );
  zPexSetCoeff( pex, 2, (1-b-a)*p2*q2 + (1-a-c)*p2*r2 + (1-c-b)*q2*r2 + 4*pqr*( (1-a)*p + (1-b)*q + (1-c)*r ) );
  zPexSetCoeff( pex, 1, 2*pqr*( (1-b-a)*p*q + (1-a-c)*r*p + (1-c-b)*q*r ) );
  zPexSetCoeff( pex, 0, (1-a-b-c)*pqr*pqr );
  zPexBH( pex, ans, zTOL, 0 );
  zPexFree( pex );

  for( i=0; i<6; i++ )
    if( ( l = ans[i].re ) >= 0 ) break;
  if( i == 6 || !zIsTiny( ans[i].im ) ){
    ZRUNERROR( ZEO_ERR_FATAL );
    return NULL;
  }
  zVec3DCreate( cp, v->e[zX]/(1+l/p), v->e[zY]/(1+l/q), v->e[zZ]/(1+l/r) );
  return cp;
}

/* the closest point from a 3D point to a 3D ellipsoid. */
double zEllips3DClosest(zEllips3D *ellips, zVec3D *p, zVec3D *cp)
{
  zVec3D pi;

  if( zEllips3DPointIsInside( ellips, p, true ) ){
    zVec3DCopy( p, cp );
    return 0;
  }
  zXform3DInv( &ellips->f, p, &pi );
  _zEllips3DClosest( zEllips3DRadiusX(ellips), zEllips3DRadiusY(ellips), zEllips3DRadiusZ(ellips), &pi, cp );
  zXform3DDRC( &ellips->f, cp );
  /* distance */
  return zVec3DDist( p, cp );
}

/* distance from a point to a 3D ellipsoid. */
double zEllips3DPointDist(zEllips3D *ellips, zVec3D *p)
{
  zVec3D cp;
  return zEllips3DClosest( ellips, p, &cp );
}

/* check if a point is in an ellipsoid. */
bool zEllips3DPointIsInside(zEllips3D *ellips, zVec3D *p, bool rim)
{
  zVec3D _p;
  double l;

  zXform3DInv( &ellips->f, p, &_p );
  l = zSqr(_p.e[zX]/zEllips3DRadiusX(ellips))
    + zSqr(_p.e[zY]/zEllips3DRadiusY(ellips))
    + zSqr(_p.e[zZ]/zEllips3DRadiusZ(ellips));
  if( rim ) l += zTOL;
  return l < 1.0 ? true : false;
}

/* volume of a 3D ellipsoid. */
double zEllips3DVolume(zEllips3D *ellips)
{
  return 4.0*zPI*zEllips3DRadiusX(ellips)*zEllips3DRadiusY(ellips)*zEllips3DRadiusZ(ellips)/3.0;
}

/* inertia of a 3D ellipsoid. */
zMat3D *zEllips3DInertia(zEllips3D *ellips, zMat3D *inertia)
{
  zMat3D i;
  double vol, xx, yy, zz;

  vol = 0.2 * zEllips3DVolume( ellips );
  xx = zSqr( zEllips3DRadiusX(ellips) ) * vol;
  yy = zSqr( zEllips3DRadiusY(ellips) ) * vol;
  zz = zSqr( zEllips3DRadiusZ(ellips) ) * vol;
  zMat3DCreate( &i,
    yy+zz, 0, 0,
    0, zz+xx, 0,
    0, 0, xx+yy );
  return zRotMat3D( zFrame3DAtt(&ellips->f), &i, inertia );
}

/* convert an ellipsoid to a polyhedron. */
zPH3D* zEllips3DToPH(zEllips3D *ellips, zPH3D *ph)
{
  zVec3D *vert, tmp;
  zTri3D *face;
  double theta;
  register int i, j, k, l, n;

  if( !zPH3DAlloc( ph,
      zEllips3DDiv(ellips)*(zEllips3DDiv(ellips)-1)+2,
      zEllips3DDiv(ellips)*(zEllips3DDiv(ellips)-1)*2 ) )
    return NULL;
  vert = zPH3DVertBuf( ph );
  face = zPH3DFaceBuf( ph );
  /* -- vertices -- */
  /* north pole */
  zVec3DMul( ZVEC3DZ, zEllips3DRadiusZ(ellips), &tmp );
  zXform3D( &ellips->f, &tmp, &vert[0] );
  /* general vertices */
  for( n=1, i=1; i<zEllips3DDiv(ellips); i++ )
    for( j=0; j<zEllips3DDiv(ellips); j++, n++ ){
      theta = zPIx2 * j / zEllips3DDiv(ellips);
      zVec3DCreatePolar( &tmp, 1.0, zPI * i / zEllips3DDiv(ellips), theta );
      tmp.e[zX] *= zEllips3DRadiusX(ellips);
      tmp.e[zY] *= zEllips3DRadiusY(ellips);
      tmp.e[zZ] *= zEllips3DRadiusZ(ellips);
      zXform3D( &ellips->f, &tmp, &vert[n] );
    }
  /* south pole */
  zVec3DMul( ZVEC3DZ, -zEllips3DRadiusZ(ellips), &tmp );
  zXform3D( &ellips->f, &tmp, &vert[n] );

  /* -- faces -- */
  /* arctic faces */
  for( n=0, i=1, j=zEllips3DDiv(ellips); i<=zEllips3DDiv(ellips); j=i++ )
    zTri3DCreate( &face[n++], &vert[0], &vert[i], &vert[j] );
  /* general */
  for( i=2; i<zEllips3DDiv(ellips); i++ )
    for( j=0, k=(i-1)*zEllips3DDiv(ellips)+1, l=i*zEllips3DDiv(ellips); j<zEllips3DDiv(ellips); j++, l=k++ ){
      zTri3DCreate( &face[n++],
        &vert[l], &vert[k], &vert[k-zEllips3DDiv(ellips)] );
      zTri3DCreate( &face[n++],
        &vert[l], &vert[k-zEllips3DDiv(ellips)], &vert[l-zEllips3DDiv(ellips)] );
    }
  /* antarctic faces */
  for( i=zPH3DVertNum(ph)-zEllips3DDiv(ellips)-1, j=zPH3DVertNum(ph)-2;
       n<zPH3DFaceNum(ph); j=i++ )
    zTri3DCreate( &face[n++],
      &vert[i], &vert[zPH3DVertNum(ph)-1], &vert[j] );

  return ph;
}

/* (static)
 * scan a 3D ellipsoid (internal function).
 */
bool _zEllips3DFScan(FILE *fp, void *instance, char *buf, bool *success)
{
  zVec3D a;

  if( strcmp( buf, "center" ) == 0 ){
    zVec3DFScan( fp, zEllips3DCenter( (zEllips3D *)instance ) );
  } else if( strcmp( buf, "ax" ) == 0 ){
    zVec3DFScan( fp, &a );
    zVec3DNormalize( &a, zEllips3DAxis( (zEllips3D *)instance, zX ) );
  } else if( strcmp( buf, "ay" ) == 0 ){
    zVec3DFScan( fp, &a );
    zVec3DNormalize( &a, zEllips3DAxis( (zEllips3D *)instance, zY ) );
  } else if( strcmp( buf, "az" ) == 0 ){
    zVec3DFScan( fp, &a );
    zVec3DNormalize( &a, zEllips3DAxis( (zEllips3D *)instance, zZ ) );
  } else if( strcmp( buf, "rx" ) == 0 ){
    zEllips3DSetRadiusX( (zEllips3D *)instance, zFDouble( fp ) );
  } else if( strcmp( buf, "ry" ) == 0 ){
    zEllips3DSetRadiusY( (zEllips3D *)instance, zFDouble( fp ) );
  } else if( strcmp( buf, "rz" ) == 0 ){
    zEllips3DSetRadiusZ( (zEllips3D *)instance, zFDouble( fp ) );
  } else if( strcmp( buf, "div" ) == 0 ){
    zEllips3DSetDiv( (zEllips3D *)instance, zFInt( fp ) );
  } else
    return false;
  return true;
}

/* scan a 3D ellipsoid from a file. */
zEllips3D *zEllips3DFScan(FILE *fp, zEllips3D *ellips)
{
  zEllips3DInit( ellips );
  zFieldFScan( fp, _zEllips3DFScan, ellips );
  return ellips;
}

/* print a 3D ellipsoid to a file. */
void zEllips3DFPrint(FILE *fp, zEllips3D *ellips)
{
  fprintf( fp, "center: " );
  zVec3DFPrint( fp, zEllips3DCenter( ellips ) );
  fprintf( fp, "ax: " );
  zVec3DFPrint( fp, zEllips3DAxis( ellips, zX ) );
  fprintf( fp, "ay: " );
  zVec3DFPrint( fp, zEllips3DAxis( ellips, zY ) );
  fprintf( fp, "az: " );
  zVec3DFPrint( fp, zEllips3DAxis( ellips, zZ ) );
  fprintf( fp, "rx: %.10g\n", zEllips3DRadiusX( ellips ) );
  fprintf( fp, "ry: %.10g\n", zEllips3DRadiusY( ellips ) );
  fprintf( fp, "rz: %.10g\n", zEllips3DRadiusZ( ellips ) );
  fprintf( fp, "div: %d\n", zEllips3DDiv( ellips ) );
}

/* methods for abstraction */

static void *_zPrim3DInitEllips(void* prim){
  return zEllips3DInit( prim ); }
static void *_zPrim3DCloneEllips(void *src, void *dest){
  return zEllips3DCopy( src, dest ); }
static void *_zPrim3DMirrorEllips(void *src, void *dest, zAxis axis){
  return zEllips3DMirror( src, dest, axis ); }
static void _zPrim3DDestroyEllips(void *prim){}
static void *_zPrim3DXformEllips(void *src, zFrame3D *f, void *dest){
  return zEllips3DXform( src, f, dest ); }
static void *_zPrim3DXformInvEllips(void *src, zFrame3D *f, void *dest){
  return zEllips3DXformInv( src, f, dest ); }
static double _zPrim3DClosestEllips(void *prim, zVec3D *p, zVec3D *cp){
  return zEllips3DClosest( prim, p, cp ); }
static double _zPrim3DPointDistEllips(void *prim, zVec3D *p){
  return zEllips3DPointDist( prim, p ); }
static bool _zPrim3DPointIsInsideEllips(void *prim, zVec3D *p, bool rim){
  return zEllips3DPointIsInside( prim, p, rim ); }
static double _zPrim3DVolumeEllips(void *prim){
  return zEllips3DVolume( prim ); }
static zVec3D *_zPrim3DBarycenterEllips(void *prim, zVec3D *c){
  zVec3DCopy( zEllips3DCenter((zEllips3D*)prim), c ); return c; }
static zMat3D *_zPrim3DInertiaEllips(void *prim, zMat3D *i){
  return zEllips3DInertia( prim, i ); }
static void _zPrim3DBaryInertiaEllips(void *prim, zVec3D *c, zMat3D *i){
  zVec3DCopy( zEllips3DCenter((zEllips3D*)prim), c );
  zEllips3DInertia( prim, i ); }
static zPH3D *_zPrim3DToPHEllips(void *prim, zPH3D *ph){
  return zEllips3DToPH( prim, ph ); }
static void *_zPrim3DFScanEllips(FILE *fp, void *prim){
  return zEllips3DFScan( fp, prim ); }
static void _zPrim3DFPrintEllips(FILE *fp, void *prim){
  return zEllips3DFPrint( fp, prim ); }

zPrimCom zprim_ellips3d_com = {
  _zPrim3DInitEllips,
  _zPrim3DCloneEllips,
  _zPrim3DMirrorEllips,
  _zPrim3DDestroyEllips,
  _zPrim3DXformEllips,
  _zPrim3DXformInvEllips,
  _zPrim3DClosestEllips,
  _zPrim3DPointDistEllips,
  _zPrim3DPointIsInsideEllips,
  _zPrim3DVolumeEllips,
  _zPrim3DBarycenterEllips,
  _zPrim3DInertiaEllips,
  _zPrim3DBaryInertiaEllips,
  _zPrim3DToPHEllips,
  _zPrim3DFScanEllips,
  _zPrim3DFPrintEllips,
};
