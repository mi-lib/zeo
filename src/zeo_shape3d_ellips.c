/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_ellips - 3D shapes: ellipsoid.
 */

#include <zeo/zeo_shape3d.h>

/* ********************************************************** */
/* CLASS: zEllips3D
 * 3D ellipsoid class
 * ********************************************************** */

/* create a 3D ellipsoid. */
zEllips3D *zEllips3DCreate(zEllips3D *ellips, const zVec3D *c, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double rx, double ry, double rz, int div)
{
  zEllips3DSetCenter( ellips, c );
  zEllips3DSetAxis( ellips, 0, ax );
  zEllips3DSetAxis( ellips, 1, ay );
  zEllips3DSetAxis( ellips, 2, az );
  zEllips3DSetRadiusX( ellips, fabs(rx) );
  zEllips3DSetRadiusY( ellips, fabs(ry) );
  zEllips3DSetRadiusZ( ellips, fabs(rz) );
  zEllips3DSetDiv( ellips, div == 0 ? ZEO_SHAPE_DEFAULT_DIV : div );
  return ellips;
}

/* initialize a 3D ellipsoid. */
zEllips3D *zEllips3DInit(zEllips3D *ellips)
{
  return zEllips3DCreateAlign( ellips, ZVEC3DZERO, 0, 0, 0, 0 );
}

/* allocate memory for a 3D ellipsoid. */
ZDEF_ALLOC_FUNCTION( zEllips3D )

/* copy a 3D ellipsoid to another. */
zEllips3D *zEllips3DCopy(const zEllips3D *src, zEllips3D *dest)
{
  return zEllips3DCreate( dest, zEllips3DCenter(src),
    zEllips3DAxis(src,0), zEllips3DAxis(src,1), zEllips3DAxis(src,2),
    zEllips3DRadiusX(src), zEllips3DRadiusY(src), zEllips3DRadiusZ(src),
    zEllips3DDiv(src) );
}

/* mirror a 3D ellipsoid. */
zEllips3D *zEllips3DMirror(const zEllips3D *src, zEllips3D *dest, zAxis axis)
{
  zEllips3DCopy( src, dest );
  zEllips3DCenter(dest)->e[(int)axis] *= -1;
  zEllips3DAxis(dest,0)->e[(int)axis] *= -1;
  zEllips3DAxis(dest,1)->e[(int)axis] *= -1;
  zEllips3DAxis(dest,2)->e[(int)axis] *= -1;
  return dest;
}

/* transform coordinates of a 3D ellipsoid. */
zEllips3D *zEllips3DXform(const zEllips3D *src, const zFrame3D *f, zEllips3D *dest)
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
zEllips3D *zEllips3DXformInv(const zEllips3D *src, const zFrame3D *f, zEllips3D *dest)
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

/* the closest point from a 3D point to an aligned 3D ellipsoid. */
static zVec3D *_zEllips3DClosest(double rx, double ry, double rz, const zVec3D *v, zVec3D *cp)
{
  double a, b, c, p, q, r, p2, q2, r2, pqr, l;
  zPex pex;
  zCVec ans;
  int i;

  a = zSqr( v->e[zX] / rx );
  b = zSqr( v->e[zY] / ry );
  c = zSqr( v->e[zZ] / rz );
  p = rx * rx; p2 = p * p;
  q = ry * ry; q2 = q * q;
  r = rz * rz; r2 = r * r; pqr = p * q * r;
  pex = zPexAlloc( 6 );
  ans = zCVecAlloc( 6 );
  if( !pex || !ans ) return NULL;
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
    if( ( l = zCVecElemNC(ans,i)->re ) >= 0 ) break;
  if( i == 6 || !zIsTiny( zCVecElemNC(ans,i)->im ) ){
    ZRUNERROR( ZEO_ERR_FATAL );
    return NULL;
  }
  zCVecFree( ans );
  zVec3DCreate( cp, v->e[zX]/(1+l/p), v->e[zY]/(1+l/q), v->e[zZ]/(1+l/r) );
  return cp;
}

/* the closest point from a 3D point to a 3D ellipsoid. */
double zEllips3DClosest(const zEllips3D *ellips, const zVec3D *p, zVec3D *cp)
{
  zVec3D pi;

  if( zEllips3DPointIsInside( ellips, p, zTOL ) ){
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
double zEllips3DDistFromPoint(const zEllips3D *ellips, const zVec3D *p)
{
  zVec3D cp;
  return zEllips3DClosest( ellips, p, &cp );
}

/* check if a point is in an ellipsoid. */
bool zEllips3DPointIsInside(const zEllips3D *ellips, const zVec3D *p, double margin)
{
  zVec3D _p;

  zXform3DInv( &ellips->f, p, &_p );
  return zSqr(_p.c.x / ( zEllips3DRadiusX(ellips) + margin ) )
       + zSqr(_p.c.y / ( zEllips3DRadiusY(ellips) + margin ) )
       + zSqr(_p.c.z / ( zEllips3DRadiusZ(ellips) + margin ) ) < 1 ? true : false;
}

/* volume of a 3D ellipsoid. */
double zEllips3DVolume(const zEllips3D *ellips)
{
  return 4.0*zPI*zEllips3DRadiusX(ellips)*zEllips3DRadiusY(ellips)*zEllips3DRadiusZ(ellips)/3.0;
}

/* inertia tensor about barycenter of a 3D ellipsoid from mass. */
zMat3D *zEllips3DBaryInertiaMass(const zEllips3D *ellips, double mass, zMat3D *inertia)
{
  zMat3D i;
  double c, xx, yy, zz;

  c = 0.2 * mass;
  xx = zSqr( zEllips3DRadiusX(ellips) ) * c;
  yy = zSqr( zEllips3DRadiusY(ellips) ) * c;
  zz = zSqr( zEllips3DRadiusZ(ellips) ) * c;
  zMat3DCreate( &i,
    yy+zz, 0, 0,
    0, zz+xx, 0,
    0, 0, xx+yy );
  return zRotMat3D( zFrame3DAtt(&ellips->f), &i, inertia );
}

/* inertia tensor about barycenter of a 3D ellipsoid. */
zMat3D *zEllips3DBaryInertia(const zEllips3D *ellips, double density, zMat3D *inertia)
{
  return zEllips3DBaryInertiaMass( ellips, density * zEllips3DVolume( ellips ), inertia );
}

/* convert an ellipsoid to a polyhedron. */
zPH3D* zEllips3DToPH(const zEllips3D *ellips, zPH3D *ph)
{
  zVec3D *vert, tmp;
  zTri3D *face;
  double theta;
  int i, j, k, l, n;

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

/* parse ZTK format */

static zVec3D *_zEllips3DAxisFromZTK(zEllips3D *ellips, int i0, int i1, int i2, ZTK *ztk)
{
  if( ZTKValCmp( ztk, "auto" ) )
    zVec3DOuterProd( zEllips3DAxis(ellips,i1), zEllips3DAxis(ellips,i2), zEllips3DAxis(ellips,i0) );
  else
    zVec3DFromZTK( zEllips3DAxis(ellips,i0), ztk );
  zVec3DNormalizeDRC( zEllips3DAxis(ellips,i0) );
  return zEllips3DAxis(ellips,i0);
}

static void *_zEllips3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zEllips3DCenter((zEllips3D*)obj), ztk );
  return obj; }
static void *_zEllips3DAxisXFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  _zEllips3DAxisFromZTK( (zEllips3D*)obj, 0, 1, 2, ztk );
  return obj; }
static void *_zEllips3DAxisYFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  _zEllips3DAxisFromZTK( (zEllips3D*)obj, 1, 2, 0, ztk );
  return obj; }
static void *_zEllips3DAxisZFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  _zEllips3DAxisFromZTK( (zEllips3D*)obj, 2, 0, 1, ztk );
  return obj; }
static void *_zEllips3DRadiusXFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DRadiusX((zEllips3D*)obj) = ZTKDouble(ztk);
  return obj; }
static void *_zEllips3DRadiusYFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DRadiusY((zEllips3D*)obj) = ZTKDouble(ztk);
  return obj; }
static void *_zEllips3DRadiusZFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DRadiusZ((zEllips3D*)obj) = ZTKDouble(ztk);
  return obj; }
static void *_zEllips3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DDiv((zEllips3D*)obj) = zShape3DDivFromZTK(ztk);
  return obj; }

static bool _zEllips3DCenterFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zEllips3DCenter((zEllips3D*)obj) );
  return true; }
static bool _zEllips3DAxisXFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zEllips3DAxis((zEllips3D*)obj,zX) );
  return true; }
static bool _zEllips3DAxisYFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zEllips3DAxis((zEllips3D*)obj,zY) );
  return true; }
static bool _zEllips3DAxisZFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zEllips3DAxis((zEllips3D*)obj,zZ) );
  return true; }
static bool _zEllips3DRadiusXFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zEllips3DRadiusX((zEllips3D*)obj) );
  return true; }
static bool _zEllips3DRadiusYFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zEllips3DRadiusY((zEllips3D*)obj) );
  return true; }
static bool _zEllips3DRadiusZFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zEllips3DRadiusZ((zEllips3D*)obj) );
  return true; }
static bool _zEllips3DDivFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zEllips3DDiv((zEllips3D*)obj) );
  return true; }

static const ZTKPrp __ztk_prp_shape_ellips[] = {
  { ZTK_KEY_ZEO_SHAPE3D_CENTER, 1, _zEllips3DCenterFromZTK, _zEllips3DCenterFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_AX,     1, _zEllips3DAxisXFromZTK, _zEllips3DAxisXFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_AY,     1, _zEllips3DAxisYFromZTK, _zEllips3DAxisYFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_AZ,     1, _zEllips3DAxisZFromZTK, _zEllips3DAxisZFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_RX,     1, _zEllips3DRadiusXFromZTK, _zEllips3DRadiusXFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_RY,     1, _zEllips3DRadiusYFromZTK, _zEllips3DRadiusYFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_RZ,     1, _zEllips3DRadiusZFromZTK, _zEllips3DRadiusZFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_DIV,    1, _zEllips3DDivFromZTK, _zEllips3DDivFPrintZTK },
};

/* print a 3D ellipsoid out to a file in a ZTK format. */
void zEllips3DFPrintZTK(FILE *fp, const zEllips3D *ellips)
{
  _ZTKPrpKeyFPrint( fp, (void*)ellips, __ztk_prp_shape_ellips );
}

/* methods for abstraction */

static void *_zShape3DEllipsInit(void *body){
  return zEllips3DInit( (zEllips3D*)body ); }
static void *_zShape3DEllipsAlloc(void){
  return zEllips3DAlloc(); }
static void *_zShape3DEllipsClone(void *src){
  zEllips3D *cln;
  return ( cln = zEllips3DAlloc() ) ? zEllips3DCopy( (zEllips3D*)src, cln ) : NULL; }
static void *_zShape3DEllipsMirror(void *src, zAxis axis){
  zEllips3D *mrr;
  return ( mrr = zEllips3DAlloc() ) ? zEllips3DMirror( (zEllips3D*)src, mrr, axis ) : NULL; }
static void _zShape3DEllipsDestroy(void *body){}
static void *_zShape3DEllipsXform(void *src, const zFrame3D *f, void *dest){
  return zEllips3DXform( (zEllips3D*)src, f, (zEllips3D*)dest ); }
static void *_zShape3DEllipsXformInv(void *src, const zFrame3D *f, void *dest){
  return zEllips3DXformInv( (zEllips3D*)src, f, (zEllips3D*)dest ); }
static double _zShape3DEllipsClosest(void *body, const zVec3D *p, zVec3D *cp){
  return zEllips3DClosest( (zEllips3D*)body, p, cp ); }
static double _zShape3DEllipsDistFromPoint(void *body, const zVec3D *p){
  return zEllips3DDistFromPoint( (zEllips3D*)body, p ); }
static bool _zShape3DEllipsPointIsInside(void *body, const zVec3D *p, double margin){
  return zEllips3DPointIsInside( (zEllips3D*)body, p, margin ); }
static double _zShape3DEllipsVolume(void *body){
  return zEllips3DVolume( (zEllips3D*)body ); }
static zVec3D *_zShape3DEllipsBarycenter(void *body, zVec3D *c){
  zVec3DCopy( zEllips3DCenter((zEllips3D*)body), c ); return c; }
static zMat3D *_zShape3DEllipsBaryInertiaMass(void *body, double mass, zMat3D *i){
  return zEllips3DBaryInertiaMass( (zEllips3D*)body, mass, i ); }
static zMat3D *_zShape3DEllipsBaryInertia(void *body, double density, zMat3D *i){
  return zEllips3DBaryInertia( (zEllips3D*)body, density, i ); }
static zPH3D *_zShape3DEllipsToPH(void *body, zPH3D *ph){
  return zEllips3DToPH( (zEllips3D*)body, ph ); }
static void *_zShape3DEllipsParseZTK(void *body, ZTK *ztk){
  zEllips3DInit( (zEllips3D*)body );
  return _ZTKEvalKey( body, NULL, ztk, __ztk_prp_shape_ellips ); }
static void _zShape3DEllipsFPrintZTK(FILE *fp, void *body){
  zEllips3DFPrintZTK( fp, (zEllips3D*)body ); }

zShape3DCom zeo_shape3d_ellips_com = {
  "ellipsoid",
  _zShape3DEllipsInit,
  _zShape3DEllipsAlloc,
  _zShape3DEllipsClone,
  _zShape3DEllipsMirror,
  _zShape3DEllipsDestroy,
  _zShape3DEllipsXform,
  _zShape3DEllipsXformInv,
  _zShape3DEllipsClosest,
  _zShape3DEllipsDistFromPoint,
  _zShape3DEllipsPointIsInside,
  _zShape3DEllipsVolume,
  _zShape3DEllipsBarycenter,
  _zShape3DEllipsBaryInertiaMass,
  _zShape3DEllipsBaryInertia,
  _zShape3DEllipsToPH,
  _zShape3DEllipsParseZTK,
  _zShape3DEllipsFPrintZTK,
};

/* create a 3D shape as an ellipsoid. */
zShape3D *zShape3DEllipsCreate(zShape3D *shape, const zVec3D *c, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double rx, double ry, double rz, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zEllips3DAlloc() ) ) return NULL;
  zEllips3DCreate( zShape3DEllips(shape), c, ax, ay, az, rx, ry, rz, div );
  shape->com = &zeo_shape3d_ellips_com;
  return shape;
}

/* create a 3D shape as an axis-aligned ellipsoid. */
zShape3D *zShape3DEllipsCreateAlign(zShape3D *shape, const zVec3D *c, double rx, double ry, double rz, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zEllips3DAlloc() ) ) return NULL;
  zEllips3DCreateAlign( zShape3DEllips(shape), c, rx, ry, rz, div );
  shape->com = &zeo_shape3d_ellips_com;
  return shape;
}
