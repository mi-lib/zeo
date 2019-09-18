/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_sphere - 3D shapes: sphere.
 */

#include <zeo/zeo_shape.h>

/* ********************************************************** */
/* CLASS: zSphere3D
 * 3D sphere class
 * ********************************************************** */

static bool _zSphere3DFScan(FILE *fp, void *instance, char *buf, bool *success);

/* create a 3D sphere. */
zSphere3D *zSphere3DCreate(zSphere3D *sphere, zVec3D *c, double r, int div)
{
  zSphere3DSetCenter( sphere, c );
  zSphere3DSetRadius( sphere, r );
  zSphere3DSetDiv( sphere, div == 0 ? ZEO_SHAPE_DEFAULT_DIV : div );
  return sphere;
}

/* initialize a 3D sphere. */
zSphere3D *zSphere3DInit(zSphere3D *sphere)
{
  return zSphere3DCreate( sphere, ZVEC3DZERO, 0, 0 );
}

/* allocate memory for a 3D sphere. */
zSphere3D *zSphere3DAlloc(void)
{
  zSphere3D *sphere;

  if( !( sphere = zAlloc( zSphere3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  return sphere;
}

/* copy a 3D sphere. */
zSphere3D *zSphere3DCopy(zSphere3D *src, zSphere3D *dest)
{
  return zSphere3DCreate( dest,
    zSphere3DCenter(src), zSphere3DRadius(src), zSphere3DDiv(src) );
}

/* mirror a 3D sphere. */
zSphere3D *zSphere3DMirror(zSphere3D *src, zSphere3D *dest, zAxis axis)
{
  zSphere3DCopy( src, dest );
  zSphere3DCenter(dest)->e[axis] *= -1;
  return dest;
}

/* transform coordinates of a 3D sphere. */
zSphere3D *zSphere3DXform(zSphere3D *src, zFrame3D *f, zSphere3D *dest)
{
  zXform3D( f, zSphere3DCenter(src), zSphere3DCenter(dest) );
  zSphere3DSetRadius( dest, zSphere3DRadius(src) );
  zSphere3DSetDiv( dest, zSphere3DDiv(src) );
  return dest;
}

/* inversely transform coordinates of a 3D sphere. */
zSphere3D *zSphere3DXformInv(zSphere3D *src, zFrame3D *f, zSphere3D *dest)
{
  zXform3DInv( f, zSphere3DCenter(src), zSphere3DCenter(dest) );
  zSphere3DSetRadius( dest, zSphere3DRadius(src) );
  zSphere3DSetDiv( dest, zSphere3DDiv(src) );
  return dest;
}

/* closest point from a point to a sphere. */
double zSphere3DClosest(zSphere3D *sphere, zVec3D *p, zVec3D *cp)
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
double zSphere3DPointDist(zSphere3D *sphere, zVec3D *p)
{
  zVec3D d;

  zVec3DSub( p, zSphere3DCenter(sphere), &d );
  return zVec3DNorm(&d) - zSphere3DRadius(sphere);
}

/* judge if a point is inside of a 3D sphere. */
bool zSphere3DPointIsInside(zSphere3D *sphere, zVec3D *p, bool rim)
{
  return zSphere3DPointDist( sphere, p ) < ( rim ? zTOL : 0 ) ? true : false;
}

/* volume of a 3D sphere. */
double zSphere3DVolume(zSphere3D *sphere)
{
  return 4.0 * zPI * pow(zSphere3DRadius(sphere),3) / 3.0;
}

/* inertia of a 3D sphere. */
zMat3D *zSphere3DInertia(zSphere3D *sphere, zMat3D *inertia)
{
  double vol, i;

  vol = zSphere3DVolume( sphere );
  i = 0.4 * zSqr( zSphere3DRadius(sphere) ) * vol;
  zMat3DCreate( inertia, i, 0, 0, 0, i, 0, 0, 0, i );
  return inertia;
}

/* convert a sphere to a polyhedron. */
zPH3D *zSphere3DToPH(zSphere3D *sphere, zPH3D *ph)
{
  zVec3D *vert;
  zTri3D *face;
  double theta;
  register int i, j, k, l, n;

  if( !zPH3DAlloc( ph,
      zSphere3DDiv(sphere)*(zSphere3DDiv(sphere)-1)+2,
      zSphere3DDiv(sphere)*(zSphere3DDiv(sphere)-1)*2 ) )
    return NULL;
  vert = zPH3DVertBuf( ph );
  face = zPH3DFaceBuf( ph );

  /* north pole vertex */
  zVec3DCopy( zSphere3DCenter(sphere), &vert[0] );
  vert[0].e[zZ] += zSphere3DRadius(sphere);
  /* general vertices */
  for( n=1, i=1; i<zSphere3DDiv(sphere); i++ )
    for( j=0; j<zSphere3DDiv(sphere); j++, n++ ){
      theta = zPIx2 * j / zSphere3DDiv(sphere);
      zVec3DCreatePolar( &vert[n], zSphere3DRadius(sphere),
        zPI * i / zSphere3DDiv(sphere), theta );
      zVec3DAddDRC( &vert[n], zSphere3DCenter(sphere) );
    }
  /* south pole vertex */
  zVec3DCopy( zSphere3DCenter(sphere), &vert[n] );
  vert[n].e[zZ] -= zSphere3DRadius(sphere);

  /* arctic faces */
  for( n=0, i=1, j=zSphere3DDiv(sphere); i<=zSphere3DDiv(sphere); j=i++ )
    zTri3DCreate( &face[n++], &vert[0], &vert[i], &vert[j] );
  /* general */
  for( i=2; i<zSphere3DDiv(sphere); i++ )
    for( j=0, k=(i-1)*zSphere3DDiv(sphere)+1, l=i*zSphere3DDiv(sphere); j<zSphere3DDiv(sphere); j++, l=k++ ){
      zTri3DCreate( &face[n++],
        &vert[l], &vert[k], &vert[k-zSphere3DDiv(sphere)] );
      zTri3DCreate( &face[n++],
        &vert[l], &vert[k-zSphere3DDiv(sphere)], &vert[l-zSphere3DDiv(sphere)] );
    }
  /* antarctic faces */
  for( i=zPH3DVertNum(ph)-zSphere3DDiv(sphere)-1, j=zPH3DVertNum(ph)-2;
       n<zPH3DFaceNum(ph); j=i++ )
    zTri3DCreate( &face[n++],
      &vert[i], &vert[zPH3DVertNum(ph)-1], &vert[j] );

  return ph;
}

/* fit a sphere to point cloud. */
zSphere3D *zSphere3DFit(zSphere3D *s, zVec3DList *pc)
{
  zMat c;
  zVec e, d;
  zVec3DListCell *p;
  int iter = 0;
  register int i, j;

  c = zMatAlloc( zListNum(pc), 4 );
  e = zVecAlloc( zListNum(pc) );
  d = zVecAlloc( 4 );
  /* initial guess */
  zSphere3DInit( s );
  zVec3DBarycenterPL( pc, zSphere3DCenter(s) );
  zSphere3DSetRadius( s, 0 );
  zListForEach( pc, p )
    zSphere3DRadius(s) += zVec3DDist( p->data, zSphere3DCenter(s) );
  zSphere3DRadius(s) /= zListNum(pc);
  /* iterative fitting */
  ZITERINIT( iter );
  for( i=0; i<iter; i++ ){
    j = 0;
    zListForEach( pc, p ){
      zMatElemNC(c,j,0) = p->data->e[zX] - zSphere3DCenter(s)->e[zX];
      zMatElemNC(c,j,1) = p->data->e[zY] - zSphere3DCenter(s)->e[zY];
      zMatElemNC(c,j,2) = p->data->e[zZ] - zSphere3DCenter(s)->e[zZ];
      zMatElemNC(c,j,3) = zSphere3DRadius(s);
      zVecElemNC(e,j)   = zVec3DSqrDist(p->data,zSphere3DCenter(s)) - zSqr(zSphere3DRadius(s));
      j++;
    }
    zLESolveErrorMin( c, e, NULL, d );
    if( zVecIsTiny( d ) ) goto TERMINATE;
    zVec3DCatDRC( zSphere3DCenter(s), 0.5, (zVec3D*)&zVecElemNC(d,0) );
    zSphere3DRadius(s) += 0.5*zVecElemNC(d,3);
  }
  ZITERWARN( iter );

 TERMINATE:
  zMatFree( c );
  return s;
}

/* parse ZTK format */

static void *_zSphere3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zSphere3DCenter((zSphere3D*)obj), ztk );
  return obj;
}
static void *_zSphere3DRadiusFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zSphere3DRadius((zSphere3D*)obj) = ZTKDouble(ztk);
  return obj;
}
static void *_zSphere3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zSphere3DDiv((zSphere3D*)obj) = zShape3DDivFromZTK(ztk);
  return obj; }

static void _zSphere3DCenterFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zSphere3DCenter((zSphere3D*)obj) ); }
static void _zSphere3DRadiusFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zSphere3DRadius((zSphere3D*)obj) ); }
static void _zSphere3DDivFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zSphere3DDiv((zSphere3D*)obj) ); }

static ZTKPrp __ztk_prp_shape_sphere[] = {
  { "center", 1, _zSphere3DCenterFromZTK, _zSphere3DCenterFPrint },
  { "radius", 1, _zSphere3DRadiusFromZTK, _zSphere3DRadiusFPrint },
  { "div", 1, _zSphere3DDivFromZTK, _zSphere3DDivFPrint },
};

#if 0
/* register a definition of tag-and-keys for a 3D sphere to a ZTK format processor. */
bool zSphere3DDefRegZTK(ZTK *ztk, char *tag)
{
  return ZTKDefRegPrp( ztk, tag, __ztk_prp_shape_sphere );
}

/* read a 3D sphere from a ZTK format processor. */
zSphere3D *zSphere3DFromZTK(zSphere3D *sphere, ZTK *ztk)
{
  zSphere3DInit( sphere );
  return ZTKEncodeKey( sphere, NULL, ztk, __ztk_prp_shape_sphere );
}
#endif

/* scan information of a 3D sphere from a file. */
bool _zSphere3DFScan(FILE *fp, void *instance, char *buf, bool *success)
{
  if( strcmp( buf, "center" ) == 0 ){
    zVec3DFScan( fp, zSphere3DCenter( (zSphere3D *)instance ) );
  } else if( strcmp( buf, "radius" ) == 0 ){
    zSphere3DSetRadius( (zSphere3D *)instance, zFDouble( fp ) );
  } else
    return false;
  return true;
}

/* scan information of a 3D sphere from a file. */
zSphere3D *zSphere3DFScan(FILE *fp, zSphere3D *sphere)
{
  zSphere3DInit( sphere );
  zFieldFScan( fp, _zSphere3DFScan, sphere );
  return sphere;
}

#if 0
/* print out a 3D sphere to a file. */
void zSphere3DFPrint(FILE *fp, zSphere3D *sphere)
{
  ZTKPrpKeyFPrint( fp, sphere, __ztk_prp_shape_sphere );
}
#endif

/* methods for abstraction */

static void *_zShape3DSphereInit(void* shape){
  return zSphere3DInit( shape ); }
static void *_zShape3DSphereAlloc(void){
  return zSphere3DAlloc(); }
static void *_zShape3DSphereClone(void *src){
  zSphere3D *cln;
  return ( cln = zSphere3DAlloc() ) ? zSphere3DCopy( src, cln ) : NULL; }
static void *_zShape3DSphereMirror(void *src, zAxis axis){
  zSphere3D *mrr;
  return ( mrr = zSphere3DAlloc() ) ? zSphere3DMirror( src, mrr, axis ) : NULL; }
static void _zShape3DSphereDestroy(void *shape){}
static void *_zShape3DSphereXform(void *src, zFrame3D *f, void *dest){
  return zSphere3DXform( src, f, dest ); }
static void *_zShape3DSphereXformInv(void *src, zFrame3D *f, void *dest){
  return zSphere3DXformInv( src, f, dest ); }
static double _zShape3DSphereClosest(void *shape, zVec3D *p, zVec3D *cp){
  return zSphere3DClosest( shape, p, cp ); }
static double _zShape3DSpherePointDist(void *shape, zVec3D *p){
  return zSphere3DPointDist( shape, p ); }
static bool _zShape3DSpherePointIsInside(void *shape, zVec3D *p, bool rim){
  return zSphere3DPointIsInside( shape, p, rim ); }
static double _zShape3DSphereVolume(void *shape){
  return zSphere3DVolume( shape ); }
static zVec3D *_zShape3DSphereBarycenter(void *shape, zVec3D *c){
  zVec3DCopy( zSphere3DCenter((zSphere3D*)shape), c ); return c; }
static zMat3D *_zShape3DSphereInertia(void *shape, zMat3D *i){
  return zSphere3DInertia( shape, i ); }
static void _zShape3DSphereBaryInertia(void *shape, zVec3D *c, zMat3D *i){
  zVec3DCopy( zSphere3DCenter((zSphere3D*)shape), c );
  zSphere3DInertia( shape, i ); }
static zPH3D *_zShape3DSphereToPH(void *shape, zPH3D *ph){
  return zSphere3DToPH( shape, ph ); }
static bool _zShape3DSphereRegZTK(ZTK *ztk, char *tag){
  return ZTKDefRegPrp( ztk, tag, __ztk_prp_shape_sphere ); }
static void *_zShape3DSphereParseZTK(void *shape, ZTK *ztk){
  zSphere3DInit( shape );
  return ZTKEncodeKey( shape, NULL, ztk, __ztk_prp_shape_sphere ); }
static void *_zShape3DSphereFScan(FILE *fp, void *shape){
  return zSphere3DFScan( fp, shape ); }
static void _zShape3DSphereFPrint(FILE *fp, void *shape){
  ZTKPrpKeyFPrint( fp, shape, __ztk_prp_shape_sphere ); }

zShape3DCom zeo_shape3d_sphere_com = {
  "sphere",
  _zShape3DSphereInit,
  _zShape3DSphereAlloc,
  _zShape3DSphereClone,
  _zShape3DSphereMirror,
  _zShape3DSphereDestroy,
  _zShape3DSphereXform,
  _zShape3DSphereXformInv,
  _zShape3DSphereClosest,
  _zShape3DSpherePointDist,
  _zShape3DSpherePointIsInside,
  _zShape3DSphereVolume,
  _zShape3DSphereBarycenter,
  _zShape3DSphereInertia,
  _zShape3DSphereBaryInertia,
  _zShape3DSphereToPH,
  _zShape3DSphereRegZTK,
  _zShape3DSphereParseZTK,
  _zShape3DSphereFScan,
  _zShape3DSphereFPrint,
};

/* create a 3D shape as a sphere. */
zShape3D *zShape3DSphereCreate(zShape3D *shape, zVec3D *c, double r, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zSphere3DAlloc() ) ) return NULL;
  zSphere3DCreate( zShape3DSphere(shape), c, r, div );
  shape->com = &zeo_shape3d_sphere_com;
  return shape;
}
