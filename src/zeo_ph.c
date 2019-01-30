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

/* zPH3DInit
 * - initialization of 3D polyhedron.
 */
zPH3D *zPH3DInit(zPH3D *ph)
{
  zArrayInit( &ph->vert );
  zArrayInit( &ph->face );
  return ph;
}

/* zPH3DAlloc
 * - allocatation of 3D polyhedron.
 */
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

/* zPH3DClone
 * - clone 3D polyhedron.
 */
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

/* zPH3DMirror
 * - mirror 3D polyhedron.
 */
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

/* zPH3DDestroy
 * - destruction of 3D polyhedron.
 */
void zPH3DDestroy(zPH3D *ph)
{
  if( !ph ) return;
  zFree( zPH3DVertBuf( ph ) );
  zFree( zPH3DFaceBuf( ph ) );
  zPH3DInit( ph );
}

/* zPH3DXfer
 * - transformation of 3D polyhedron.
 *   NOTE: it assumes that 'src' is already cloned to 'dest'.
 */
zPH3D *zPH3DXfer(zPH3D *src, zFrame3D *f, zPH3D *dest)
{
  register int i;

  for( i=0; i<zPH3DVertNum(dest); i++ )
    zXfer3D( f, zPH3DVert(src,i), zPH3DVert(dest,i) );
  for( i=0; i<zPH3DFaceNum(dest); i++ )
    zTri3DCalcNorm( zPH3DFace(dest,i) );
  return dest;
}

/* zPH3DXferInv
 * - inverse transformation of 3D polyhedron.
 *   NOTE: it assumes that 'src' is already cloned to 'dest'.
 */
zPH3D *zPH3DXferInv(zPH3D *src, zFrame3D *f, zPH3D *dest)
{
  register int i;

  for( i=0; i<zPH3DVertNum(dest); i++ )
    zXfer3DInv( f, zPH3DVert(src,i), zPH3DVert(dest,i) );
  for( i=0; i<zPH3DFaceNum(dest); i++ )
    zTri3DCalcNorm( zPH3DFace(dest,i) );
  return dest;
}

/* zPH3DContigVert
 * - contiguous vertix of 3D polyhedron to a point.
 */
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

/* zPH3DClosest
 * - the closest point to 3D polyhedron.
 */
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

/* zPH3DPointDist
 * - distance from a point to 3D polyhedron.
 */
double zPH3DPointDist(zPH3D *ph, zVec3D *p)
{
  zVec3D cp;
  return zPH3DClosest( ph, p, &cp );
}

/* zPH3DPointIsInside
 * - check if a point is inside of a polyhedron.
 */
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

/* zPH3DVolume
 * - volume of 3D polyhedron.
 */
double zPH3DVolume(zPH3D *ph)
{
  register int i;
  double v;

  for( v=0, i=0; i<zPH3DFaceNum(ph); i++ )
    v += zTri3DConeVolume( zPH3DFace(ph,i), ZVEC3DZERO );
  return v;
}

/* zPH3DBarycenter
 * - barycenter of 3D polyhedron.
 */
zVec3D *zPH3DBarycenter(zPH3D *ph, zVec3D *c)
{
  register int i;
  zVec3D bc;
  double v, vol;

  zVec3DClear( c );
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

/* zPH3DInertia
 * - inertia tensor of 3D polyhedron.
 */
zMat3D *zPH3DInertia(zPH3D *ph, zMat3D *inertia)
{
  register int j;
  zMat3D i;

  zMat3DClear( inertia );
  for( j=0; j<zPH3DFaceNum(ph); j++ ){
    zTri3DConeInertia( zPH3DFace(ph,j), &i );
    _zMat3DAddDRC( inertia, &i );
  }
  return inertia;
}

/* zPH3DBaryInertia
 * - inertia about barycenter of 3D polyhedron.
 */
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
 * _zPH3DLoopNext
 * - next vertex of bottom loop.
 */
int _zPH3DLoopNext(int n, int i){
  if( ++i == n ) i = 0;
  return i;
}

/* (static)
 * _zPH3DSweepBottom
 * - triangulate bottom loop.
 */
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

/* zPH3DPrism
 * - create prism by extrusion.
 */
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

/* zPH3DPyramid
 * - create pyramid.
 */
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

/* zPH3DTorus
 * - create torus.
 */
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

/* zPH3DLathe
 * - create solid revolution by lathe.
 */
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

typedef struct{
  zPH3D *ph;
  int vc;
  int fc;
} _zPH3DParam;

static bool _zPH3DFRead(FILE *fp, void *instance, char *buf, bool *success);

/* (static)
 * _zPH3DFRead
 * - input of 3D polyhedron from file.
 */
bool _zPH3DFRead(FILE *fp, void *instance, char *buf, bool *success)
{
  _zPH3DParam *prm;
  int v1, v2, v3;

  prm = instance;
  if( strcmp( buf, "vert" ) == 0 ){
    if( prm->vc >= zPH3DVertNum(prm->ph) ) return false;
    if( !zFToken( fp, buf, BUFSIZ ) ) /* identifier of a vertex */
      return ( *success = false );
    zVec3DFRead( fp, zPH3DVert(prm->ph,prm->vc++) );
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

/* zPH3DFRead
 * - input of 3D polyhedron from file.
 */
zPH3D *zPH3DFRead(FILE *fp, zPH3D *ph)
{
  _zPH3DParam prm;

  zPH3DInit( ph );
  prm.vc = zFCountKey( fp, "vert" );
  prm.fc = zFCountKey( fp, "face" );
  if( !zPH3DAlloc( ph, prm.vc, prm.fc ) ) return NULL;
  prm.ph = ph;
  prm.vc = prm.fc = 0;
  if( zFieldFRead( fp, _zPH3DFRead, &prm ) ) return ph;
  zPH3DDestroy( ph );
  return NULL;
}

/* zPH3DFWrite
 * - output of 3D polyhedron to file.
 */
void zPH3DFWrite(FILE *fp, zPH3D *ph)
{
  register int i;

  if( !ph || zPH3DVertNum(ph) == 0 ) return;
  for( i=0; i<zPH3DVertNum(ph); i++ ){
    fprintf( fp, "vert %d: ", i );
    zVec3DFWrite( fp, zPH3DVert(ph,i) );
  }
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    fprintf( fp, "face " );
    fprintf( fp, "%d %d %d\n",
      (int)( zPH3DFaceVert(ph,i,0)-zPH3DVertBuf(ph) ),
      (int)( zPH3DFaceVert(ph,i,1)-zPH3DVertBuf(ph) ),
      (int)( zPH3DFaceVert(ph,i,2)-zPH3DVertBuf(ph) ) );
  }
  fprintf( fp, "\n" );
}
