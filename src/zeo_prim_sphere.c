/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_sphere - primitive 3D shapes: sphere.
 */

#include <zeo/zeo_prim.h>

/* ********************************************************** */
/* CLASS: zSphere3D
 * 3D sphere class
 * ********************************************************** */

static bool _zSphere3DFRead(FILE *fp, void *instance, char *buf, bool *success);

/* zSphere3DCreate
 * - create 3D sphere.
 */
zSphere3D *zSphere3DCreate(zSphere3D *sphere, zVec3D *c, double r, int div)
{
  zSphere3DSetCenter( sphere, c );
  zSphere3DSetRadius( sphere, r );
  zSphere3DSetDiv( sphere, div == 0 ? ZEO_PRIM_DEFAULT_DIV : div );
  return sphere;
}

/* zSphere3DInit
 * - initialize 3D sphere.
 */
zSphere3D *zSphere3DInit(zSphere3D *sphere)
{
  return zSphere3DCreate( sphere, ZVEC3DZERO, 0, 0 );
}

/* zSphere3DCopy
 * - copy 3D sphere.
 */
zSphere3D *zSphere3DCopy(zSphere3D *src, zSphere3D *dest)
{
  return zSphere3DCreate( dest,
    zSphere3DCenter(src), zSphere3DRadius(src), zSphere3DDiv(src) );
}

/* zSphere3DMirror
 * - mirror 3D sphere.
 */
zSphere3D *zSphere3DMirror(zSphere3D *src, zSphere3D *dest, zAxis axis)
{
  zSphere3DCopy( src, dest );
  zSphere3DCenter(dest)->e[axis] *= -1;
  return dest;
}

/* zSphere3DXfer
 * - transform 3D sphere.
 */
zSphere3D *zSphere3DXfer(zSphere3D *src, zFrame3D *f, zSphere3D *dest)
{
  zXfer3D( f, zSphere3DCenter(src), zSphere3DCenter(dest) );
  zSphere3DSetRadius( dest, zSphere3DRadius(src) );
  zSphere3DSetDiv( dest, zSphere3DDiv(src) );
  return dest;
}

/* zSphere3DXferInv
 * - inversely transform 3D sphere.
 */
zSphere3D *zSphere3DXferInv(zSphere3D *src, zFrame3D *f, zSphere3D *dest)
{
  zXfer3DInv( f, zSphere3DCenter(src), zSphere3DCenter(dest) );
  zSphere3DSetRadius( dest, zSphere3DRadius(src) );
  zSphere3DSetDiv( dest, zSphere3DDiv(src) );
  return dest;
}

/* zSphere3DClosest
 * - the closest point from 3D point to 3D sphere.
 */
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

/* zSphere3DPointDist
 * - distance between 3D sphere and a point.
 */
double zSphere3DPointDist(zSphere3D *sphere, zVec3D *p)
{
  zVec3D d;

  zVec3DSub( p, zSphere3DCenter(sphere), &d );
  return zVec3DNorm(&d) - zSphere3DRadius(sphere);
}

/* zSphere3DPointIsInside
 * - judge if a point is inside of 3D sphere.
 */
bool zSphere3DPointIsInside(zSphere3D *sphere, zVec3D *p, bool rim)
{
  return zSphere3DPointDist( sphere, p ) < ( rim ? zTOL : 0 ) ? true : false;
}

/* zSphere3DVolume
 * - volume of 3D sphere.
 */
double zSphere3DVolume(zSphere3D *sphere)
{
  return 4.0 * zPI * pow(zSphere3DRadius(sphere),3) / 3.0;
}

/* zSphere3DInertia
 * - inertia of 3D sphere.
 */
zMat3D *zSphere3DInertia(zSphere3D *sphere, zMat3D *inertia)
{
  double vol, i;

  vol = zSphere3DVolume( sphere );
  i = 0.4 * zSqr( zSphere3DRadius(sphere) ) * vol;
  zMat3DCreate( inertia, i, 0, 0, 0, i, 0, 0, 0, i );
  return inertia;
}

/* zSphere3DToPH
 * - convert sphere to polyhedron.
 */
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
  /* -- vertices -- */
  /* north pole */
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
  /* south pole */
  zVec3DCopy( zSphere3DCenter(sphere), &vert[n] );
  vert[n].e[zZ] -= zSphere3DRadius(sphere);

  /* -- faces -- */
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

/* zSphere3DFit
 * - fit a sphere to point cloud.
 */
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

/* (static)
 * _zSphere3DFRead
 * - input of 3D sphere.
 */
bool _zSphere3DFRead(FILE *fp, void *instance, char *buf, bool *success)
{
  if( strcmp( buf, "center" ) == 0 ){
    zVec3DFRead( fp, zSphere3DCenter( (zSphere3D *)instance ) );
  } else if( strcmp( buf, "radius" ) == 0 ){
    zSphere3DSetRadius( (zSphere3D *)instance, zFDouble( fp ) );
  } else
    return false;
  return true;
}

/* zSphere3DFRead
 * - input of 3D sphere.
 */
zSphere3D *zSphere3DFRead(FILE *fp, zSphere3D *sphere)
{
  zSphere3DInit( sphere );
  zFieldFRead( fp, _zSphere3DFRead, sphere );
  return sphere;
}

/* zSphere3DFWrite
 * - output of 3D sphere.
 */
void zSphere3DFWrite(FILE *fp, zSphere3D *sphere)
{
  fprintf( fp, "center: " );
  zVec3DFWrite( fp, zSphere3DCenter( sphere ) );
  fprintf( fp, "radius: %.10g\n", zSphere3DRadius( sphere ) );
}

/* methods for abstraction */

static void *_zPrim3DInitSphere(void* prim){
  return zSphere3DInit( prim ); }
static void *_zPrim3DCloneSphere(void *src, void *dest){
  return zSphere3DCopy( src, dest ); }
static void *_zPrim3DMirrorSphere(void *src, void *dest, zAxis axis){
  return zSphere3DMirror( src, dest, axis ); }
static void _zPrim3DDestroySphere(void *prim){}
static void *_zPrim3DXferSphere(void *src, zFrame3D *f, void *dest){
  return zSphere3DXfer( src, f, dest ); }
static void *_zPrim3DXferInvSphere(void *src, zFrame3D *f, void *dest){
  return zSphere3DXferInv( src, f, dest ); }
static double _zPrim3DClosestSphere(void *prim, zVec3D *p, zVec3D *cp){
  return zSphere3DClosest( prim, p, cp ); }
static double _zPrim3DPointDistSphere(void *prim, zVec3D *p){
  return zSphere3DPointDist( prim, p ); }
static bool _zPrim3DPointIsInsideSphere(void *prim, zVec3D *p, bool rim){
  return zSphere3DPointIsInside( prim, p, rim ); }
static double _zPrim3DVolumeSphere(void *prim){
  return zSphere3DVolume( prim ); }
static zVec3D *_zPrim3DBarycenterSphere(void *prim, zVec3D *c){
  zVec3DCopy( zSphere3DCenter((zSphere3D*)prim), c ); return c; }
static zMat3D *_zPrim3DInertiaSphere(void *prim, zMat3D *i){
  return zSphere3DInertia( prim, i ); }
static void _zPrim3DBaryInertiaSphere(void *prim, zVec3D *c, zMat3D *i){
  zVec3DCopy( zSphere3DCenter((zSphere3D*)prim), c );
  zSphere3DInertia( prim, i ); }
static zPH3D *_zPrim3DToPHSphere(void *prim, zPH3D *ph){
  return zSphere3DToPH( prim, ph ); }
static void *_zPrim3DFReadSphere(FILE *fp, void *prim){
  return zSphere3DFRead( fp, prim ); }
static void _zPrim3DFWriteSphere(FILE *fp, void *prim){
  return zSphere3DFWrite( fp, prim ); }

zPrimCom zprim_sphere3d_com = {
  _zPrim3DInitSphere,
  _zPrim3DCloneSphere,
  _zPrim3DMirrorSphere,
  _zPrim3DDestroySphere,
  _zPrim3DXferSphere,
  _zPrim3DXferInvSphere,
  _zPrim3DClosestSphere,
  _zPrim3DPointDistSphere,
  _zPrim3DPointIsInsideSphere,
  _zPrim3DVolumeSphere,
  _zPrim3DBarycenterSphere,
  _zPrim3DInertiaSphere,
  _zPrim3DBaryInertiaSphere,
  _zPrim3DToPHSphere,
  _zPrim3DFReadSphere,
  _zPrim3DFWriteSphere,
};
