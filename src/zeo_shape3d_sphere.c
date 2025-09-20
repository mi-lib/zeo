/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_sphere - 3D shapes: sphere.
 */

#include <zeo/zeo_shape3d.h>
#include <zeo/zeo_vec3d_data.h>

/* convert a sphere to a polyhedron. */
zPH3D *zSphere3DToPH(const zSphere3D *sphere, zPH3D *ph)
{
  zVec3D *vert;
  zTri3D *face;
  double theta;
  int i, j, k, l, n;

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
zSphere3D *zSphere3DFit(zSphere3D *sphere, zVec3DData *data)
{
  zMat c;
  zVec e, d;
  zVec3D *v;
  int i, j, iter = 0;

  c = zMatAlloc( zVec3DDataSize(data), 4 );
  e = zVecAlloc( zVec3DDataSize(data) );
  d = zVecAlloc( 4 );
  /* initial guess */
  zSphere3DInit( sphere );
  zVec3DDataBarycenter( data, zSphere3DCenter(sphere) );
  zSphere3DSetRadius( sphere, 0 );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) )
    zSphere3DRadius(sphere) += zVec3DDist( v, zSphere3DCenter(sphere) );
  zSphere3DRadius(sphere) /= zVec3DDataSize(data);
  /* iterative fitting */
  ZITERINIT( iter );
  for( i=0; i<iter; i++ ){
    j = 0;
    zVec3DDataRewind( data );
    while( ( v = zVec3DDataFetch( data ) ) ){
      zMatElemNC(c,j,0) = v->c.x - zSphere3DCenter(sphere)->c.x;
      zMatElemNC(c,j,1) = v->c.y - zSphere3DCenter(sphere)->c.y;
      zMatElemNC(c,j,2) = v->c.z - zSphere3DCenter(sphere)->c.z;
      zMatElemNC(c,j,3) = zSphere3DRadius(sphere);
      zVecElemNC(e,j)   = zVec3DSqrDist(v,zSphere3DCenter(sphere)) - zSqr(zSphere3DRadius(sphere));
      j++;
    }
    zLESolveErrorMin( c, e, NULL, d );
    if( zVecIsTiny( d ) ) goto TERMINATE;
    zVec3DCatDRC( zSphere3DCenter(sphere), 0.5, (zVec3D*)&zVecElemNC(d,0) );
    zSphere3DRadius(sphere) += 0.5*zVecElemNC(d,3);
  }
  ZITERWARN( iter );

 TERMINATE:
  zMatFree( c );
  return sphere;
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

static bool _zSphere3DCenterFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zSphere3DCenter((zSphere3D*)obj) );
  return true; }
static bool _zSphere3DRadiusFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zSphere3DRadius((zSphere3D*)obj) );
  return true; }
static bool _zSphere3DDivFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zSphere3DDiv((zSphere3D*)obj) );
  return true; }

static const ZTKPrp __ztk_prp_shape_sphere[] = {
  { ZTK_KEY_ZEO_SHAPE3D_CENTER, 1, _zSphere3DCenterFromZTK, _zSphere3DCenterFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_RADIUS, 1, _zSphere3DRadiusFromZTK, _zSphere3DRadiusFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_DIV,    1, _zSphere3DDivFromZTK, _zSphere3DDivFPrintZTK },
};

/* print a 3D sphere out to a file in a ZTK format. */
void zSphere3DFPrintZTK(FILE *fp, const zSphere3D *sphere)
{
  _ZTKPrpKeyFPrint( fp, (void*)sphere, __ztk_prp_shape_sphere );
}

/* methods for abstraction */

static void *_zShape3DSphereInit(void *body){
  return zSphere3DInit( (zSphere3D*)body ); }
static void *_zShape3DSphereAlloc(void){
  return zSphere3DAlloc(); }
static void *_zShape3DSphereClone(void *src){
  zSphere3D *cln;
  return ( cln = zSphere3DAlloc() ) ? zSphere3DCopy( (zSphere3D*)src, cln ) : NULL; }
static void *_zShape3DSphereMirror(void *src, zAxis axis){
  zSphere3D *mrr;
  return ( mrr = zSphere3DAlloc() ) ? zSphere3DMirror( (zSphere3D*)src, mrr, axis ) : NULL; }
static void _zShape3DSphereDestroy(void *body){}
static void *_zShape3DSphereXform(void *src, const zFrame3D *f, void *dest){
  return zSphere3DXform( (zSphere3D*)src, f, (zSphere3D*)dest ); }
static void *_zShape3DSphereXformInv(void *src, const zFrame3D *f, void *dest){
  return zSphere3DXformInv( (zSphere3D*)src, f, (zSphere3D*)dest ); }
static double _zShape3DSphereClosest(void *body, const zVec3D *p, zVec3D *cp){
  return zSphere3DClosest( (zSphere3D*)body, p, cp ); }
static double _zShape3DSphereDistFromPoint(void *body, const zVec3D *p){
  return zSphere3DDistFromPoint( (zSphere3D*)body, p ); }
static bool _zShape3DSpherePointIsInside(void *body, const zVec3D *p, double margin){
  return zSphere3DPointIsInside( (zSphere3D*)body, p, margin ); }
static double _zShape3DSphereVolume(void *body){
  return zSphere3DVolume( (zSphere3D*)body ); }
static zVec3D *_zShape3DSphereBarycenter(void *body, zVec3D *c){
  zVec3DCopy( zSphere3DCenter((zSphere3D*)body), c ); return c; }
static zMat3D *_zShape3DSphereBaryInertiaMass(void *body, double mass, zMat3D *i){
  return zSphere3DBaryInertiaMass( (zSphere3D*)body, mass, i ); }
static zMat3D *_zShape3DSphereBaryInertia(void *body, double density, zMat3D *i){
  return zSphere3DBaryInertia( (zSphere3D*)body, density, i ); }
static zPH3D *_zShape3DSphereToPH(void *body, zPH3D *ph){
  return zSphere3DToPH( (zSphere3D*)body, ph ); }
static void *_zShape3DSphereParseZTK(void *body, ZTK *ztk){
  zSphere3DInit( (zSphere3D*)body );
  return _ZTKEvalKey( body, NULL, ztk, __ztk_prp_shape_sphere ); }
static void _zShape3DSphereFPrintZTK(FILE *fp, void *body){
  zSphere3DFPrintZTK( fp, (zSphere3D*)body ); }

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
  _zShape3DSphereDistFromPoint,
  _zShape3DSpherePointIsInside,
  _zShape3DSphereVolume,
  _zShape3DSphereBarycenter,
  _zShape3DSphereBaryInertiaMass,
  _zShape3DSphereBaryInertia,
  _zShape3DSphereToPH,
  _zShape3DSphereParseZTK,
  _zShape3DSphereFPrintZTK,
};

/* create a 3D shape as a sphere. */
zShape3D *zShape3DSphereCreate(zShape3D *shape, const zVec3D *center, double radius, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zSphere3DAlloc() ) ) return NULL;
  zSphere3DCreate( zShape3DSphere(shape), center, radius, div );
  shape->com = &zeo_shape3d_sphere_com;
  return shape;
}
