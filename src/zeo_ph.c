/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph - polyhedron
 */

#include <zeo/zeo_ph.h>

/* ********************************************************** */
/* CLASS: zPH3D
 * 3D polyhedron class
 * ********************************************************** */

/* initialize a 3D polyhedron. */
zPH3D *zPH3DInit(zPH3D *ph)
{
  zArrayInit( &ph->vert );
  zArrayInit( &ph->face );
  return ph;
}

/* allocate a 3D polyhedron. */
zPH3D *zPH3DAlloc(zPH3D *ph, int vn, int fn)
{
  zPH3DInit( ph );
  if( vn > 0 ){ /* vertices */
    zArrayAlloc( &ph->vert, zVec3D, vn );
    if( !zPH3DVertBuf(ph) ) goto ERROR;
  }
  if( fn > 0 ){ /* faces */
    zArrayAlloc( &ph->face, zTri3D, fn );
    if( !zPH3DFaceBuf(ph) ) goto ERROR;
  }
  return ph;

 ERROR:
  ZALLOCERROR();
  zPH3DDestroy( ph );
  return NULL;
}

/* clone a 3D polyhedron. */
zPH3D *zPH3DClone(zPH3D *src, zPH3D *dest)
{
  register int i;

  if( !zPH3DAlloc( dest, zPH3DVertNum(src), zPH3DFaceNum(src) ) )
    return NULL;
  for( i=0; i<zPH3DVertNum(src); i++ )
    zVec3DCopy( zPH3DVert(src,i), zPH3DVert(dest,i) );
  for( i=0; i<zPH3DFaceNum(src); i++ )
    zTri3DCreate( zPH3DFace(dest,i),
      zPH3DVert(dest,zPH3DFaceVert(src,i,0)-zPH3DVertBuf(src)),
      zPH3DVert(dest,zPH3DFaceVert(src,i,1)-zPH3DVertBuf(src)),
      zPH3DVert(dest,zPH3DFaceVert(src,i,2)-zPH3DVertBuf(src)) );
  return dest;
}

/* mirror a 3D polyhedron. */
zPH3D *zPH3DMirror(zPH3D *src, zPH3D *dest, zAxis axis)
{
  register int i;

  if( !zPH3DClone( src, dest ) ) return NULL;
  for( i=0; i<zPH3DVertNum(dest); i++ )
    zPH3DVert(dest,i)->e[axis] *= -1;
  for( i=0; i<zPH3DFaceNum(dest); i++ ){
    zSwap( zVec3D*, zPH3DFaceVert(dest,i,1), zPH3DFaceVert(dest,i,2) );
    zVec3DRevDRC( zPH3DFaceNorm(dest,i) );
    zTri3DCalcNorm( zPH3DFace(dest,i) );
  }
  return dest;
}

/* destroy a 3D polyhedron. */
void zPH3DDestroy(zPH3D *ph)
{
  if( !ph ) return;
  zFree( zPH3DVertBuf( ph ) );
  zFree( zPH3DFaceBuf( ph ) );
  zPH3DInit( ph );
}

/* transform coordinates of a 3D polyhedron.
 * NOTE: it assumes that 'src' is already cloned to 'dest'. */
zPH3D *zPH3DXform(zPH3D *src, zFrame3D *f, zPH3D *dest)
{
  register int i;

  for( i=0; i<zPH3DVertNum(dest); i++ )
    zXform3D( f, zPH3DVert(src,i), zPH3DVert(dest,i) );
  for( i=0; i<zPH3DFaceNum(dest); i++ )
    zTri3DCalcNorm( zPH3DFace(dest,i) );
  return dest;
}

/* inversely transform coordinates of a 3D polyhedron.
 * NOTE: it assumes that src is already cloned to dest. */
zPH3D *zPH3DXformInv(zPH3D *src, zFrame3D *f, zPH3D *dest)
{
  register int i;

  for( i=0; i<zPH3DVertNum(dest); i++ )
    zXform3DInv( f, zPH3DVert(src,i), zPH3DVert(dest,i) );
  for( i=0; i<zPH3DFaceNum(dest); i++ )
    zTri3DCalcNorm( zPH3DFace(dest,i) );
  return dest;
}

/* contiguous vertix of a 3D polyhedron to a point. */
zVec3D *zPH3DContigVert(zPH3D *ph, zVec3D *p, double *d)
{
  register int i;
  zVec3D *v, *nv;
  double _d, dmin;

  if( zPH3DVertNum(ph) == 0 ) return NULL;
  if( !d ) d = &_d;
  v = zPH3DVert(ph,0);
  dmin = zVec3DDist( p, v );
  for( i=1; i<zPH3DVertNum(ph); i++ ){
    if( ( *d = zVec3DDist( ( nv = zPH3DVert(ph,i) ), p ) ) < dmin ){
      v = nv;
      dmin = *d;
    }
  }
  *d = dmin;
  return v;
}

/* the closest point to a 3D polyhedron. */
double zPH3DClosest(zPH3D *ph, zVec3D *p, zVec3D *cp)
{
  register int i;
  zVec3D ncp;
  double d, dmin;

  if( zPH3DFaceNum(ph) == 0 ){
    ZRUNWARN( ZEO_ERR_NOFACE );
    zVec3DCopy( p, cp );
    return 0;
  }
  dmin = zTri3DClosest( zPH3DFace(ph,0), p, cp );
  for( i=1; i<zPH3DFaceNum(ph); i++ )
    if( ( d = zTri3DClosest( zPH3DFace(ph,i), p, &ncp ) ) < dmin ){
      zVec3DCopy( &ncp, cp );
      dmin = d;
    }
  return dmin;
}

/* distance from a point to a 3D polyhedron. */
double zPH3DPointDist(zPH3D *ph, zVec3D *p)
{
  zVec3D cp;
  return zPH3DClosest( ph, p, &cp );
}

/* check if a point is inside of a polyhedron. */
bool zPH3DPointIsInside(zPH3D *ph, zVec3D *p, bool rim)
{
  register int i;
  double tol;

  tol = rim ? zTOL : 0;
  for( i=0; i<zPH3DFaceNum(ph); i++ )
    if( zTri3DPointDist( zPH3DFace(ph,i), p ) >= tol )
      return false;
  return true;
}

/* volume of a 3D polyhedron. */
double zPH3DVolume(zPH3D *ph)
{
  register int i;
  double v;

  for( v=0, i=0; i<zPH3DFaceNum(ph); i++ )
    v += zTri3DConeVolume( zPH3DFace(ph,i), ZVEC3DZERO );
  return v;
}

/* barycenter of a 3D polyhedron. */
zVec3D *zPH3DBarycenter(zPH3D *ph, zVec3D *c)
{
  register int i;
  zVec3D bc;
  double v, vol;

  zVec3DZero( c );
  for( vol=0, i=0; i<zPH3DFaceNum(ph); i++ ){
    v = zTri3DConeVolume( zPH3DFace(ph,i), ZVEC3DZERO );
    vol += v;
    zTri3DConeBarycenter( zPH3DFace(ph,i), ZVEC3DZERO, &bc );
    zVec3DMulDRC( &bc, v );
    zVec3DAddDRC( c, &bc );
  }
  zVec3DDivDRC( c, vol );
  return c;
}

/* inertia tensor of a 3D polyhedron. */
zMat3D *zPH3DInertia(zPH3D *ph, zMat3D *inertia)
{
  register int j;
  zMat3D i;

  zMat3DZero( inertia );
  for( j=0; j<zPH3DFaceNum(ph); j++ ){
    zTri3DConeInertia( zPH3DFace(ph,j), &i );
    _zMat3DAddDRC( inertia, &i );
  }
  return inertia;
}

/* inertia about barycenter of a 3D polyhedron. */
void zPH3DBaryInertia(zPH3D *ph, zVec3D *c, zMat3D *i)
{
  zMat3D m;

  zPH3DBarycenter( ph, c );
  zVec3DTripleProd2Mat3D( c, c, &m );
  zMat3DMulDRC( &m, zPH3DVolume( ph ) );
  zPH3DInertia( ph, i );
  _zMat3DAddDRC( i, &m );
}

/* solid modeling */
static int _zPH3DLoopNext(int n, int i);
static int _zPH3DSweepBottom(zVec3D v[], int n, zTri3D f[], zVec3D *ref);

/* (static)
 * next vertex of the bottom loop. */
int _zPH3DLoopNext(int n, int i){
  if( ++i == n ) i = 0;
  return i;
}

/* (static)
 * triangulate the bottom loop. */
int _zPH3DSweepBottom(zVec3D v[], int n, zTri3D f[], zVec3D *ref)
{
  zTri3DList tlist;
  int nf;

  nf = zTriangulate( v, n, &tlist );
  zTri3DListAlign( &tlist, ref );
  zTri3DListCopyArray( &tlist, f, nf );
  zTri3DListDestroy( &tlist );
  return nf;
}

/* create a prism by extrusion from the bottom loop. */
zPH3D *zPH3DPrism(zPH3D *prism, zVec3D bottom[], int n, zVec3D *shift)
{
  register int i, i1;
  int nf;
  zTri3D *(*_tri)(zTri3D*,zVec3D*,zVec3D*,zVec3D*) = zTri3DCreate;
  zTri3D *fbuf;
  zVec3D ref;

  if( !zPH3DAlloc( prism, n*2, 4*(n-1) ) ) return NULL;
  /* vertices */
  for( i=0; i<n; i++ ){
    zVec3DCopy( &bottom[i], zPH3DVert(prism,i) );
    zVec3DAdd( &bottom[i], shift, zPH3DVert(prism,i+n) );
  }
  /* bottom faces */
  zVec3DRev( shift, &ref );
  nf = _zPH3DSweepBottom( zPH3DVert(prism,0), n, zPH3DFace(prism,0), &ref );
  zVec3DRevDRC( &ref );
  nf+= _zPH3DSweepBottom( zPH3DVert(prism,n), n, zPH3DFace(prism,nf), &ref );

  if( nf > 2*(n-2) ) goto FATAL_ERROR;
  /* side faces */
  i = zPH3DFaceVert(prism,0,0) - zPH3DVertBuf(prism);
  i1 = _zPH3DLoopNext( n, i );
  if( zPH3DFaceVert(prism,0,1)-zPH3DVertBuf(prism) == i1 )
    _tri = zTri3DCreateRev; /* direct faces outwards */
  else if( zPH3DFaceVert(prism,0,2)-zPH3DVertBuf(prism) != i1 )
    goto FATAL_ERROR;
  for( i=0; i<n; i++, nf+=2 ){
    i1 = _zPH3DLoopNext(n,i);
    _tri( zPH3DFace(prism,nf),
      zPH3DVert(prism,i), zPH3DVert(prism,i1)  , zPH3DVert(prism,i1+n) );
    _tri( zPH3DFace(prism,nf+1),
      zPH3DVert(prism,i), zPH3DVert(prism,i1+n), zPH3DVert(prism,i+n) );
  }
  if( nf < 4*(n-1) )
    if( ( fbuf = zRealloc( zPH3DFaceBuf(prism), zTri3D, nf ) ) ){
      zPH3DSetFaceBuf( prism, fbuf );
      zPH3DSetFaceNum( prism, nf );
    }
  return prism;
 FATAL_ERROR:
  ZRUNERROR( ZEO_ERR_FATAL );
  zPH3DDestroy( prism );
  return NULL;
}

/* create a pyramid from the bottom loop and a vertex. */
zPH3D *zPH3DPyramid(zPH3D *pyr, zVec3D bottom[], int n, zVec3D *vert)
{
  register int i, i1;
  int nf;
  zTri3D *(*_tri)(zTri3D*,zVec3D*,zVec3D*,zVec3D*) = zTri3DCreate;
  zTri3D *fbuf;
  zVec3D ref;

  if( !zPH3DAlloc( pyr, n+1, 2*(n-1) ) ) return NULL;
  /* vertices */
  for( i=0; i<n; i++ )
    zVec3DCopy( &bottom[i], zPH3DVert(pyr,i) );
  zVec3DCopy( vert, zPH3DVert(pyr,n) );
  /* bottom faces */
  zVec3DSub( zPH3DVert(pyr,0), vert, &ref );
  nf = _zPH3DSweepBottom( zPH3DVert(pyr,0), n, zPH3DFace(pyr,0), &ref );

  if( nf > n-2 ) goto FATAL_ERROR;
  /* side faces */
  i = zPH3DFaceVert(pyr,0,0) - zPH3DVertBuf(pyr);
  i1 = _zPH3DLoopNext( n, i );
  if( zPH3DFaceVert(pyr,0,1)-zPH3DVertBuf(pyr) == i1 )
    _tri = zTri3DCreateRev; /* direct faces outwards */
  else if( zPH3DFaceVert(pyr,0,2)-zPH3DVertBuf(pyr) != i1 )
    goto FATAL_ERROR;
  for( i=0; i<n; i++, nf++ )
    _tri( zPH3DFace(pyr,nf),
      zPH3DVert(pyr,i), zPH3DVert(pyr,_zPH3DLoopNext(n,i)), zPH3DVert(pyr,n) );
  if( nf < 2*(n-1) )
    if( ( fbuf = zRealloc( zPH3DFaceBuf(pyr), zTri3D, nf ) ) ){
      zPH3DSetFaceBuf( pyr, fbuf );
      zPH3DSetFaceNum( pyr, nf );
    }

  return pyr;
 FATAL_ERROR:
  ZRUNERROR( ZEO_ERR_FATAL );
  zPH3DDestroy( pyr );
  return NULL;
}

/* create a torus from a section loop. */
zPH3D *zPH3DTorus(zPH3D *torus, zVec3D loop[], int n, int div, zVec3D *center, zVec3D *axis)
{
  register int i, i0, j, j0, k;
  zVec3D d, a, *v;

  if( !zPH3DAlloc( torus, n*div, 2*n*div ) ) return NULL;
  for( i0=n-1, i=0; i<n; i0=i++ ){
    zVec3DSub( &loop[i], center, &d );
    for( j0=div-1, j=0; j<div; j0=j++ ){
      /* vertices */
      zVec3DMul( axis, zPIx2 * j / div, &a );
      zVec3DRot( &d, &a, zPH3DVert( torus, ( k = i*div+j ) ) );
      zVec3DAddDRC( zPH3DVert(torus,k), center );
      /* faces */
      v = zPH3DVert( torus, i0*div+j0 );
      zTri3DCreate( zPH3DFace(torus,2*k  ),
        zPH3DVert(torus,k), zPH3DVert(torus,i0*div+j), v );
      zTri3DCreate( zPH3DFace(torus,2*k+1),
        zPH3DVert(torus,k), v, zPH3DVert(torus,i*div+j0) );
    }
  }
  /* facet direction correction */
  if( zPH3DVolume(torus) < 0 )
    for( i=0; i<zPH3DFaceNum(torus); i++ )
      zTri3DRevDRC( zPH3DFace(torus,i) );
  return torus;
}

/* create a solid revolution by lathe. */
zPH3D *zPH3DLathe(zPH3D *lathe, zVec3D rim[], int n, int div, zVec3D *center, zVec3D *axis)
{
  register int i, i0, j, j0, k;
  zVec3D d, a, *v;

  if( !zPH3DAlloc( lathe, n*div, 2*(n-1)*div+2*(div-2) ) ) return NULL;
  /* vertices */
  for( i=0; i<n; i++ ){
    zVec3DSub( &rim[i], center, &d );
    for( j=0; j<div; j++ ){
      zVec3DMul( axis, zPIx2 * j / div, &a );
      zVec3DRot( &d, &a, zPH3DVert( lathe, ( k = i*div+j ) ) );
      zVec3DAddDRC( zPH3DVert(lathe,k), center );
    }
  }
  /* side faces */
  for( i0=0, i=1; i<n; i0=i++ )
    for( j0=div-1, j=0; j<div; j0=j++ ){
      v = zPH3DVert( lathe, i*div+j0 );
      k = i0*div + j;
      zTri3DCreate( zPH3DFace(lathe,2*k  ),
        zPH3DVert(lathe,k), zPH3DVert(lathe,i0*div+j0), v );
      zTri3DCreate( zPH3DFace(lathe,2*k+1),
        zPH3DVert(lathe,k), v, zPH3DVert(lathe,i*div+j) );
    }
  /* bottom faces */
  for( k=2*(n-1)*div, i0=div-2, j0=(n-1)*div ,j=2; j<div; k++, j++ ){
    zTri3DCreate( zPH3DFace(lathe,k),
      zPH3DVert(lathe,0), zPH3DVert(lathe,j-1), zPH3DVert(lathe,j) );
    zTri3DCreate( zPH3DFace(lathe,k+i0),
      zPH3DVert(lathe,j0), zPH3DVert(lathe,j0+j-1), zPH3DVert(lathe,j0+j) );
  }
  /* facet direction correction */
  if( zPH3DVolume(lathe) < 0 )
    for( i=0; i<zPH3DFaceNum(lathe); i++ )
      zTri3DRevDRC( zPH3DFace(lathe,i) );
  return lathe;
}

/* parse ZTK format */

static void *_zPH3DVertFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  int vi;

  if( ( vi = ZTKInt(ztk) ) != i )
    ZRUNWARN( ZEO_WARN_PH_VERT_UNMATCH, vi );
  zVec3DFromZTK( zPH3DVert((zPH3D*)obj,i), ztk );
  return obj;
}

static void *_zPH3DFaceFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  int i0, i1, i2;

  if( ( i0 = ZTKInt(ztk) ) >= zPH3DVertNum((zPH3D*)obj) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_VERT_ID, i0 );
    return NULL;
  }
  if( ( i1 = ZTKInt(ztk) ) >= zPH3DVertNum((zPH3D*)obj) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_VERT_ID, i1 );
    return NULL;
  }
  if( ( i2 = ZTKInt(ztk) ) >= zPH3DVertNum((zPH3D*)obj) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_VERT_ID, i2 );
    return NULL;
  }
  zTri3DCreate( zPH3DFace((zPH3D*)obj,i),
    zPH3DVert((zPH3D*)obj,i0), zPH3DVert((zPH3D*)obj,i1), zPH3DVert((zPH3D*)obj,i2) );
  return obj;
}

static ZTKPrp __ztk_prp_ph[] = {
  { "vert", -1, _zPH3DVertFromZTK, NULL },
  { "face", -1, _zPH3DFaceFromZTK, NULL },
};

/* register a definition of tag-and-keys for a 3D polyhedron cylinder to a ZTK format processor. */
bool zPH3DDefRegZTK(ZTK *ztk, char *tag)
{
  return ZTKDefRegPrp( ztk, tag, __ztk_prp_ph );
}

/* read a 3D polyhedron from a ZTK format processor. */
zPH3D *zPH3DFromZTK(zPH3D *ph, ZTK *ztk)
{
  int num_vert, num_face;

  zPH3DInit( ph );
  if( !ZTKKeyRewind( ztk ) ) return NULL;
  if( ( num_vert = ZTKCountKey( ztk, "vert" ) ) == 0 ){
    ZRUNWARN( ZEO_WARN_PH_EMPTY );
    return NULL;
  }
  num_face = ZTKCountKey( ztk, "face" );
  zArrayAlloc( &ph->vert, zVec3D, num_vert );
  zArrayAlloc( &ph->face, zTri3D, num_face );
  if( zPH3DVertNum(ph) != num_vert ||
      zPH3DFaceNum(ph) != num_face ) return NULL;
  /* vertices & faces */
  return ZTKEncodeKey( ph, NULL, ztk, __ztk_prp_ph );
}

typedef struct{
  zPH3D *ph;
  int vc;
  int fc;
} _zPH3DParam;

static bool _zPH3DFScan(FILE *fp, void *instance, char *buf, bool *success);

/* (static)
 * scan a 3D polyhedron from a file. */
bool _zPH3DFScan(FILE *fp, void *instance, char *buf, bool *success)
{
  _zPH3DParam *prm;
  int v1, v2, v3;

  prm = instance;
  if( strcmp( buf, "vert" ) == 0 ){
    if( prm->vc >= zPH3DVertNum(prm->ph) ) return false;
    if( !zFToken( fp, buf, BUFSIZ ) ) /* identifier of a vertex */
      return ( *success = false );
    zVec3DFScan( fp, zPH3DVert(prm->ph,prm->vc++) );
  } else if( strcmp( buf, "face" ) == 0 ){
    if( prm->fc >= zPH3DFaceNum(prm->ph) ) return false;
    v1 = zFInt( fp );
    v2 = zFInt( fp );
    v3 = zFInt( fp );
    zTri3DCreate( zPH3DFace(prm->ph,prm->fc++),
      zPH3DVert(prm->ph,v1), zPH3DVert(prm->ph,v2), zPH3DVert(prm->ph,v3) );
  } else
    return false;
  return true;
}

/* scan a 3D polyhedron from a file. */
zPH3D *zPH3DFScan(FILE *fp, zPH3D *ph)
{
  _zPH3DParam prm;

  zPH3DInit( ph );
  prm.vc = zFCountKey( fp, "vert" );
  prm.fc = zFCountKey( fp, "face" );
  if( !zPH3DAlloc( ph, prm.vc, prm.fc ) ) return NULL;
  prm.ph = ph;
  prm.vc = prm.fc = 0;
  if( zFieldFScan( fp, _zPH3DFScan, &prm ) ) return ph;
  zPH3DDestroy( ph );
  return NULL;
}

/* print a 3D polyhedron to a file. */
void zPH3DFPrint(FILE *fp, zPH3D *ph)
{
  register int i;

  if( !ph || zPH3DVertNum(ph) == 0 ) return;
  for( i=0; i<zPH3DVertNum(ph); i++ ){
    fprintf( fp, "vert: %d ", i );
    zVec3DFPrint( fp, zPH3DVert(ph,i) );
  }
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    fprintf( fp, "face: %d %d %d\n",
      (int)( zPH3DFaceVert(ph,i,0)-zPH3DVertBuf(ph) ),
      (int)( zPH3DFaceVert(ph,i,1)-zPH3DVertBuf(ph) ),
      (int)( zPH3DFaceVert(ph,i,2)-zPH3DVertBuf(ph) ) );
  }
}

/* *** STL format *** */

/* STL vertex list */
typedef struct{
  int id;
  zVec3D v;
} _zSTLVert;
zListClass( _zSTLVertList, _zSTLVertListCell, _zSTLVert );

static void _zSTLVertListInit(_zSTLVertList *list);
static _zSTLVertListCell *_zSTLVertListReg(_zSTLVertList *list, zVec3D *v);
static zVertArray *_zSTLVertListToVertArray(_zSTLVertList *list, zVertArray *array);

/* initialize a STL vertex list. */
void _zSTLVertListInit(_zSTLVertList *list)
{
  zListInit( list );
  zListRoot(list)->data.id = -1;
}

/* register a vector to a STL vertex list. */
_zSTLVertListCell *_zSTLVertListReg(_zSTLVertList *list, zVec3D *v)
{
  _zSTLVertListCell *cp;

  zListForEach( list, cp )
    if( zVec3DEqual( &cp->data.v, v ) ) return cp;
  if( !( cp = zAlloc( _zSTLVertListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  cp->data.id = zListHead(list)->data.id + 1;
  zVec3DCopy( v, &cp->data.v );
  zListInsertHead( list, cp );
  return cp;
}

/* convert a STL vertex list to an array of vertices. */
zVertArray *_zSTLVertListToVertArray(_zSTLVertList *list, zVertArray *array)
{
  _zSTLVertListCell *cp;
  register int i = 0;

  zArrayAlloc( array, zVec3D, zListNum(list) );
  if( zArraySize(array) < zListNum(list) ){
    zArrayFree( array );
    return NULL;
  }
  zListForEach( list, cp )
    zVec3DCopy( &cp->data.v, zArrayElemNC(array,i++) );
  return array;
}

/* STL facet list */
typedef struct{
  _zSTLVert *v1, *v2, *v3;
  zVec3D normal;
} _zSTLFacet;
zListClass( _zSTLFacetList, _zSTLFacetListCell, _zSTLFacet );

static _zSTLFacetListCell *_zSTLFacetListReg(_zSTLFacetList *flist, _zSTLVertList *vlist, zVec3D *normal, zVec3D v[]);
static zFaceArray *_zSTLFacetListToFaceArray(_zSTLFacetList *list, zVertArray *varray, zFaceArray *array);

/* register a facet to a STL facet list. */
_zSTLFacetListCell *_zSTLFacetListReg(_zSTLFacetList *flist, _zSTLVertList *vlist, zVec3D *normal, zVec3D v[])
{
  zTri3D facet;
  _zSTLVertListCell *vc1, *vc2, *vc3;
  _zSTLFacetListCell *cp;

  if( !( vc1 = _zSTLVertListReg( vlist, &v[0] ) ) ||
      !( vc2 = _zSTLVertListReg( vlist, &v[1] ) ) ||
      !( vc3 = _zSTLVertListReg( vlist, &v[2] ) ) ) return NULL;
  if( !( cp = zAlloc( _zSTLFacetListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  cp->data.v1 = &vc1->data;
  cp->data.v2 = &vc2->data;
  cp->data.v3 = &vc3->data;
  zTri3DCreate( &facet, &vc1->data.v, &vc2->data.v, &vc3->data.v );
  if( !zVec3DEqual( zTri3DNorm(&facet), normal ) ){
    ZRUNWARN( ZEO_WARN_STL_WRONGNORMAL );
  }
  zVec3DCopy( zTri3DNorm(&facet), &cp->data.normal );
  zListInsertHead( flist, cp );
  return cp;
}

/* convert a STL facet list to an array of faces. */
zFaceArray *_zSTLFacetListToFaceArray(_zSTLFacetList *list, zVertArray *varray, zFaceArray *array)
{
  _zSTLFacetListCell *cp;
  register int i = 0;

  zArrayAlloc( array, zTri3D, zListNum(list) );
  if( zArraySize(array) < zListNum(list) ){
    zArrayFree( array );
    return NULL;
  }
  zListForEach( list, cp ){
    zTri3DSetVert( zArrayElemNC(array,i), 0, zArrayElemNC(varray,cp->data.v1->id) );
    zTri3DSetVert( zArrayElemNC(array,i), 1, zArrayElemNC(varray,cp->data.v2->id) );
    zTri3DSetVert( zArrayElemNC(array,i), 2, zArrayElemNC(varray,cp->data.v3->id) );
    zTri3DSetNorm( zArrayElemNC(array,i), &cp->data.normal );
    i++;
  }
  return array;
}

/* read/write in STL format */

static bool _zPH3DFReadSTL_ASCIIloop(FILE *fp, char buf[], zVec3D v[]);
static zPH3D *_zPH3DFReadSTL_ASCIIfacet(FILE *fp, char buf[], zPH3D *ph, _zSTLVertList *vert_list, _zSTLFacetList *facet_list);
static zPH3D *_zPH3DFReadSTL_ASCIIsolid(FILE *fp, zPH3D *ph, char name[], size_t namesize);

static zVec3D *_zPH3DFReadSTL_BinVec(FILE *fp, zVec3D *v);
static void _zPH3DFWriteSTL_BinVec(FILE *fp, zVec3D *v);

/* scan an outer loop of a face of a 3D polyhedron from ASCII STL format */
bool _zPH3DFReadSTL_ASCIIloop(FILE *fp, char buf[], zVec3D v[])
{
  int i = 0;

  if( !zFToken( fp, buf, BUFSIZ ) ){
    ZRUNERROR( ZEO_ERR_STL_INCOMPLETE );
    return false;
  }
  if( strcmp( buf, "loop" ) != 0 ){
    ZRUNWARN( ZEO_WARN_STL_MIS_LOOP );
    return false;
  }
  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ){
      ZRUNERROR( ZEO_ERR_STL_INCOMPLETE );
      return false;
    }
    if( strcmp( buf, "vertex" ) == 0 ){
      if( i > 2 ){
        ZRUNWARN( ZEO_WARN_STL_TOOMANYVERT );
        continue;
      }
      zVec3DFScan( fp, &v[i++] );
    } else
    if( strcmp( buf, "endloop" ) == 0 ){
      if( i < 3 ){
        ZRUNWARN( ZEO_WARN_STL_TOOFEWVERT );
        break;
      }
      return true;
    }
  }
  return false;
}

/* scan a facet of a 3D polyhedron from ASCII STL format */
zPH3D *_zPH3DFReadSTL_ASCIIfacet(FILE *fp, char buf[], zPH3D *ph, _zSTLVertList *vert_list, _zSTLFacetList *facet_list)
{
  zVec3D normal, v[3];

  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ){
      ZRUNERROR( ZEO_ERR_STL_INCOMPLETE );
      break;
    }
    if( strcmp( buf, "normal" ) == 0 ){
      zVec3DFScan( fp, &normal );
    } else
    if( strcmp( buf, "outer" ) == 0 ){
      _zPH3DFReadSTL_ASCIIloop( fp, buf, v );
    } else
    if( strcmp( buf, "endfacet" ) == 0 ){
      if( !_zSTLFacetListReg( facet_list, vert_list, &normal, v ) ) break;
      return ph;
    }
  }
  return NULL;
}

/* scan a solid object of a 3D polyhedron from ASCII STL format */
zPH3D *_zPH3DFReadSTL_ASCIIsolid(FILE *fp, zPH3D *ph, char name[], size_t namesize)
{
  char buf[BUFSIZ];
  _zSTLVertList vert_list;
  _zSTLFacetList facet_list;

  _zSTLVertListInit( &vert_list );
  zListInit( &facet_list );
  if( !zFToken( fp, name, namesize ) ) return NULL; /* no name */
  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ){
      ZRUNERROR( ZEO_ERR_STL_INCOMPLETE );
      break;
    }
    if( strcmp( buf, "endsolid" ) == 0 ){
      if( !_zSTLVertListToVertArray( &vert_list, &ph->vert ) ||
          !_zSTLFacetListToFaceArray( &facet_list, &ph->vert, &ph->face ) )
        ph = NULL;
      zListDestroy( _zSTLVertListCell, &vert_list );
      zListDestroy( _zSTLFacetListCell, &facet_list );
      return ph;
    } else
    if( strcmp( buf, "facet" ) != 0 ) continue; /* skip as a comment */
    if( !_zPH3DFReadSTL_ASCIIfacet( fp, buf, ph, &vert_list, &facet_list ) ) break; /* read a facet */
  }
  return NULL;
}

/* scan a 3D polyhedron from ASCII STL format */
zPH3D *zPH3DFReadSTL_ASCII(FILE *fp, zPH3D *ph, char name[], size_t namesize)
{
  char buf[BUFSIZ];

  zPH3DInit( ph );
  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ) break; /* early EOF */
    if( strcmp( buf, "solid" ) != 0 ) continue; /* skip as a comment */
    return _zPH3DFReadSTL_ASCIIsolid( fp, ph, name, namesize ); /* succeed to scan */
  }
  return NULL; /* fail to scan */
}

/* print a 3D polyhedron to ASCII STL format */
void zPH3DFWriteSTL_ASCII(FILE *fp, zPH3D *ph, char name[])
{
  register int i;

  fprintf( fp, "solid %s\n", name );
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    fprintf( fp, "\tfacet normal\t" );
    zVec3DDataFPrint( fp, zTri3DNorm(zPH3DFace(ph,i)) );
    fprintf( fp, "\n\t\touter loop" );
    fprintf( fp, "\n\t\t\tvertex\t" );
    zVec3DDataFPrint( fp, zTri3DVert(zPH3DFace(ph,i),0) );
    fprintf( fp, "\n\t\t\tvertex\t" );
    zVec3DDataFPrint( fp, zTri3DVert(zPH3DFace(ph,i),1) );
    fprintf( fp, "\n\t\t\tvertex\t" );
    zVec3DDataFPrint( fp, zTri3DVert(zPH3DFace(ph,i),2) );
    fprintf( fp, "\n\t\tendloop\n\tendfacet\n" );
  }
  fprintf( fp, "endsolid\n" );
}

/* read a 3D vector from binary STL format */
zVec3D *_zPH3DFReadSTL_BinVec(FILE *fp, zVec3D *v)
{
  float val1, val2, val3;

  if( fread( &val1, sizeof(float), 1, fp ) < 1 );
  if( fread( &val2, sizeof(float), 1, fp ) < 1 );
  if( fread( &val3, sizeof(float), 1, fp ) < 1 );
  return zVec3DCreate( v, val1, val2, val3 );
}

/* write a 3D vector to binary STL format */
void _zPH3DFWriteSTL_BinVec(FILE *fp, zVec3D *v)
{
  float val1, val2, val3;

  val1 = v->e[0];
  val2 = v->e[1];
  val3 = v->e[2];
  if( fwrite( &val1, sizeof(float), 1, fp ) < 1 ) ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( fwrite( &val2, sizeof(float), 1, fp ) < 1 ) ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( fwrite( &val3, sizeof(float), 1, fp ) < 1 ) ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
}

#define ZEO_STL_HEADSIZ 80

/* read a 3D polyhedron from binary STL format */
zPH3D *zPH3DFReadSTL_Bin(FILE *fp, zPH3D *ph, char name[])
{
  uint32_t nf; /* number of facets */
  uint16_t dummy;
  zVec3D normal, v[3];
  _zSTLVertList vert_list;
  _zSTLFacetList facet_list;
  register int i;

  _zSTLVertListInit( &vert_list );
  zListInit( &facet_list );
  zPH3DInit( ph );
  if( fread( name, sizeof(char), ZEO_STL_HEADSIZ, fp ) < ZEO_STL_HEADSIZ )
    ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( fread( &nf, sizeof(uint32_t), 1, fp ) < 1 ) ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( nf <= 0 ) return NULL;
  for( i=0; i<nf; i++ ){
    _zPH3DFReadSTL_BinVec( fp, &normal );
    _zPH3DFReadSTL_BinVec( fp, &v[0] );
    _zPH3DFReadSTL_BinVec( fp, &v[1] );
    _zPH3DFReadSTL_BinVec( fp, &v[2] );
    if( fread( &dummy, sizeof(uint16_t), 1, fp ) < 1 )
      ZRUNWARN( ZEO_WARN_STL_MISSINGDATA ); /* two empty bytes */
    if( !_zSTLFacetListReg( &facet_list, &vert_list, &normal, v ) ) break;
  }
  if( !_zSTLVertListToVertArray( &vert_list, &ph->vert ) ||
      !_zSTLFacetListToFaceArray( &facet_list, &ph->vert, &ph->face ) )
    ph = NULL;
  zListDestroy( _zSTLVertListCell, &vert_list );
  zListDestroy( _zSTLFacetListCell, &facet_list );
  return ph;
}

/* write a 3D polyhedron to binary STL format */
void zPH3DFWriteSTL_Bin(FILE *fp, zPH3D *ph, char name[])
{
  uint32_t nf;
  uint16_t dummy = 0;
  register int i;

  if( fwrite( name, sizeof(char), ZEO_STL_HEADSIZ, fp ) < ZEO_STL_HEADSIZ );
  nf = zPH3DFaceNum(ph);
  if( fwrite( &nf, sizeof(uint32_t), 1, fp ) < 1 );
  for( i=0; i<nf; i++ ){
    _zPH3DFWriteSTL_BinVec( fp, zTri3DNorm(zPH3DFace(ph,i)) );
    _zPH3DFWriteSTL_BinVec( fp, zTri3DVert(zPH3DFace(ph,i),0) );
    _zPH3DFWriteSTL_BinVec( fp, zTri3DVert(zPH3DFace(ph,i),1) );
    _zPH3DFWriteSTL_BinVec( fp, zTri3DVert(zPH3DFace(ph,i),2) );
    if( fwrite( &dummy, sizeof(uint16_t), 1, fp ) < 1 ); /* two empty bytes */
  }
}

#define ZEO_ERR_STL_UNREADABLE "File unreadable. Probably not a STL file."

/* read a 3D polyhedron from either ASCII/binary STL format */
zPH3D *zPH3DFReadSTL(FILE *fp, zPH3D *ph, char name[], size_t namesize)
{
  char buf[6];
  long int pos;

  pos = ftell( fp );
  if( fread( buf, sizeof(char), 6, fp ) < 6 ){
    ZRUNERROR( ZEO_ERR_STL_UNREADABLE );
    return NULL;
  }
  fseek( fp, pos, SEEK_SET );
  return strncmp( buf, "solid ", 6 ) == 0 ?
    zPH3DFReadSTL_ASCII( fp, ph, name, namesize ) :
    zPH3DFReadSTL_Bin( fp, ph, name );
}
