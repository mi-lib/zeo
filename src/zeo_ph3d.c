/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph3d - polyhedron.
 */

#include <zeo/zeo_ph3d.h>
#include <zeo/zeo_vec2d.h>

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
    if( !zPH3DVertBuf(ph) ) goto ZPH3DALLOC_ERROR;
  }
  if( fn > 0 ){ /* faces */
    zArrayAlloc( &ph->face, zTri3D, fn );
    if( !zPH3DFaceBuf(ph) ) goto ZPH3DALLOC_ERROR;
  }
  return ph;

 ZPH3DALLOC_ERROR:
  ZALLOCERROR();
  zPH3DDestroy( ph );
  return NULL;
}

/* clone a 3D polyhedron. */
zPH3D *zPH3DClone(const zPH3D *src, zPH3D *dest)
{
  int i;

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
zPH3D *zPH3DMirror(const zPH3D *src, zPH3D *dest, zAxis axis)
{
  int i;

  if( !zPH3DClone( src, dest ) ) return NULL;
  for( i=0; i<zPH3DVertNum(dest); i++ )
    zPH3DVert(dest,i)->e[(int)axis] *= -1;
  for( i=0; i<zPH3DFaceNum(dest); i++ ){
    zSwap( zVec3D*, zPH3DFaceVert(dest,i,1), zPH3DFaceVert(dest,i,2) );
    zVec3DRevDRC( zPH3DFaceNorm(dest,i) );
    zTri3DCalcNorm( zPH3DFace(dest,i) );
  }
  return dest;
}

/* scale a 3D polyhedron. */
zPH3D *zPH3DScale(zPH3D *ph, double scale)
{
  int i;

  for( i=0; i<zPH3DVertNum(ph); i++ )
    zVec3DMulDRC( zPH3DVert(ph,i), scale );
  return ph;
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
zPH3D *zPH3DXform(const zPH3D *src, const zFrame3D *f, zPH3D *dest)
{
  int i;

  for( i=0; i<zPH3DVertNum(dest); i++ )
    zXform3D( f, zPH3DVert(src,i), zPH3DVert(dest,i) );
  for( i=0; i<zPH3DFaceNum(dest); i++ )
    zTri3DCalcNorm( zPH3DFace(dest,i) );
  return dest;
}

/* inversely transform coordinates of a 3D polyhedron.
 * NOTE: it assumes that src is already cloned to dest. */
zPH3D *zPH3DXformInv(const zPH3D *src, const zFrame3D *f, zPH3D *dest)
{
  int i;

  for( i=0; i<zPH3DVertNum(dest); i++ )
    zXform3DInv( f, zPH3DVert(src,i), zPH3DVert(dest,i) );
  for( i=0; i<zPH3DFaceNum(dest); i++ )
    zTri3DCalcNorm( zPH3DFace(dest,i) );
  return dest;
}

/* contiguous vertix of a 3D polyhedron to a point. */
zVec3D *zPH3DContigVert(const zPH3D *ph, const zVec3D *p, double *d)
{
  int i;
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

/* the closest point to a 3D polyhedron.
 * note: this is not an exact computation because it supposes that
 *  - the polyhedron is convex
 *  - the polyhedron is closed
 *  - normal vectors of all facets of the polyhedron direct outward.
 */
double zPH3DClosest(const zPH3D *ph, const zVec3D *p, zVec3D *cp)
{
  int i;
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
double zPH3DDistFromPoint(const zPH3D *ph, const zVec3D *p)
{
  zVec3D cp;
  return zPH3DClosest( ph, p, &cp );
}

/* check if a point is inside of a polyhedron. */
bool zPH3DPointIsInside(const zPH3D *ph, const zVec3D *p, double margin)
{
  int i;

  for( i=0; i<zPH3DFaceNum(ph); i++ )
    if( zTri3DDistFromPointToPlane( zPH3DFace(ph,i), p ) >= margin ) return false;
  return true;
}

/* volume of a 3D polyhedron. */
double zPH3DVolume(const zPH3D *ph)
{
  int i;
  double v;

  for( v=0, i=0; i<zPH3DFaceNum(ph); i++ )
    v += zTri3DConeVolume( zPH3DFace(ph,i), ZVEC3DZERO );
  return v;
}

/* barycenter of a 3D polyhedron. */
zVec3D *zPH3DBarycenter(const zPH3D *ph, zVec3D *c)
{
  int i;
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
zMat3D *zPH3DInertia(const zPH3D *ph, double density, zMat3D *inertia)
{
  int j;
  zMat3D i;

  zMat3DZero( inertia );
  for( j=0; j<zPH3DFaceNum(ph); j++ ){
    zTri3DConeInertia( zPH3DFace(ph,j), density, &i );
    _zMat3DAddDRC( inertia, &i );
  }
  return inertia;
}

/* inertia tensor of a 3D polyhedron from mass. */
zMat3D *zPH3DInertiaMass(const zPH3D *ph, double mass, zMat3D *inertia)
{
  return zPH3DInertia( ph, mass / zPH3DVolume( ph ), inertia );
}

/* inertia tensor about barycenter of a 3D polyhedron. */
zMat3D *zPH3DBaryInertia(const zPH3D *ph, double density, zMat3D *i)
{
  zVec3D c;

  zPH3DInertia( ph, density, i );
  zPH3DBarycenter( ph, &c );
  _zMat3DCatVec3DDoubleOuterProdDRC( i, density * zPH3DVolume( ph ), &c );
  return i;
}

/* inertia tensor about barycenter of a 3D polyhedron from mass. */
zMat3D *zPH3DBaryInertiaMass(const zPH3D *ph, double mass, zMat3D *i)
{
  zVec3D c;

  zPH3DInertiaMass( ph, mass, i );
  zPH3DBarycenter( ph, &c );
  _zMat3DCatVec3DDoubleOuterProdDRC( i, mass, &c );
  return i;
}

/* solid modeling */

/* next vertex of the bottom loop. */
static int _zPH3DLoopNext(int n, int i)
{
  if( ++i == n ) i = 0;
  return i;
}

/* triangulate the bottom loop. */
static int _zPH3DSweepBottom(zVec3DArray *va, zTri3D f[], const zVec3D *ref)
{
  zTri3DList tlist;
  int nf;

  nf = zTriangulate( va, &tlist );
  zTri3DListAlign( &tlist, ref );
  zTri3DListCopyArray( &tlist, f, nf );
  zTri3DListDestroy( &tlist );
  return nf;
}

/* create a prism by extrusion from the bottom loop. */
zPH3D *zPH3DCreatePrism(zPH3D *prism, const zVec3D bottom[], int n, const zVec3D *shift)
{
  int i, i1, nf;
  zTri3D *(*_tri)(zTri3D*,const zVec3D*,const zVec3D*,const zVec3D*) = zTri3DCreate;
  zTri3D *fbuf;
  zVec3D ref;
  zVec3DArray va;

  if( !zPH3DAlloc( prism, n*2, 4*(n-1) ) ) return NULL;
  /* vertices */
  for( i=0; i<n; i++ ){
    zVec3DCopy( &bottom[i], zPH3DVert(prism,i) );
    zVec3DAdd( &bottom[i], shift, zPH3DVert(prism,i+n) );
  }
  /* bottom faces */
  zVec3DRev( shift, &ref );
  zArraySize(&va) = n;
  zArrayBuf(&va) = zPH3DVert(prism,0);
  nf = _zPH3DSweepBottom( &va, zPH3DFace(prism,0), &ref );
  zVec3DRevDRC( &ref );
  zArrayBuf(&va) = zPH3DVert(prism,n);
  nf+= _zPH3DSweepBottom( &va, zPH3DFace(prism,nf), &ref );

  if( nf > 2*(n-2) ) goto ZPH3DPRISM_FATAL_ERROR;
  /* side faces */
  i = zPH3DFaceVert(prism,0,0) - zPH3DVertBuf(prism);
  i1 = _zPH3DLoopNext( n, i );
  if( zPH3DFaceVert(prism,0,1)-zPH3DVertBuf(prism) == i1 )
    _tri = zTri3DCreateRev; /* direct faces outwards */
  else if( zPH3DFaceVert(prism,0,2)-zPH3DVertBuf(prism) != i1 )
    goto ZPH3DPRISM_FATAL_ERROR;
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
 ZPH3DPRISM_FATAL_ERROR:
  ZRUNERROR( ZEO_ERR_FATAL );
  zPH3DDestroy( prism );
  return NULL;
}

/* create a pyramid from the bottom loop and a vertex. */
zPH3D *zPH3DCreatePyramid(zPH3D *pyr, const zVec3D bottom[], int n, const zVec3D *vert)
{
  int i, i1, nf;
  zTri3D *(*_tri)(zTri3D*,const zVec3D*,const zVec3D*,const zVec3D*) = zTri3DCreate;
  zTri3D *fbuf;
  zVec3D ref;
  zVec3DArray va;

  if( !zPH3DAlloc( pyr, n+1, 2*(n-1) ) ) return NULL;
  /* vertices */
  for( i=0; i<n; i++ )
    zVec3DCopy( &bottom[i], zPH3DVert(pyr,i) );
  zVec3DCopy( vert, zPH3DVert(pyr,n) );
  /* bottom faces */
  zVec3DSub( zPH3DVert(pyr,0), vert, &ref );
  zArraySize(&va) = n;
  zArrayBuf(&va) = zPH3DVert(pyr,0);
  nf = _zPH3DSweepBottom( &va, zPH3DFace(pyr,0), &ref );

  if( nf > n-2 ) goto ZPH3DPYRAMID_FATAL_ERROR;
  /* side faces */
  i = zPH3DFaceVert(pyr,0,0) - zPH3DVertBuf(pyr);
  i1 = _zPH3DLoopNext( n, i );
  if( zPH3DFaceVert(pyr,0,1)-zPH3DVertBuf(pyr) == i1 )
    _tri = zTri3DCreateRev; /* direct faces outwards */
  else if( zPH3DFaceVert(pyr,0,2)-zPH3DVertBuf(pyr) != i1 )
    goto ZPH3DPYRAMID_FATAL_ERROR;
  for( i=0; i<n; i++, nf++ )
    _tri( zPH3DFace(pyr,nf),
      zPH3DVert(pyr,i), zPH3DVert(pyr,_zPH3DLoopNext(n,i)), zPH3DVert(pyr,n) );
  if( nf < 2*(n-1) )
    if( ( fbuf = zRealloc( zPH3DFaceBuf(pyr), zTri3D, nf ) ) ){
      zPH3DSetFaceBuf( pyr, fbuf );
      zPH3DSetFaceNum( pyr, nf );
    }

  return pyr;
 ZPH3DPYRAMID_FATAL_ERROR:
  ZRUNERROR( ZEO_ERR_FATAL );
  zPH3DDestroy( pyr );
  return NULL;
}

/* create a torus from a section loop. */
zPH3D *zPH3DCreateTorus(zPH3D *torus, const zVec3D loop[], int n, int div, const zVec3D *center, const zVec3D *axis)
{
  int i, i0, j, j0, k;
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
zPH3D *zPH3DCreateLathe(zPH3D *lathe, const zVec3D rim[], int n, int div, const zVec3D *center, const zVec3D *axis)
{
  int i, i0, j, j0, k;
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

/* flag to print status while reading polyhedral model from a file. */
bool zeo_ph_echo_while_reading = false;

/* parse ZTK format */

static void *_zPH3DVertFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  int vi, n;
  zVec3D *v;

  if( ( vi = ZTKInt(ztk) ) != i )
    ZRUNWARN( ZEO_WARN_PH_VERT_UNMATCH, vi );
  if( zArraySize((zVec3DArray*)arg) > 0 ){
    n = zArraySize((zVec3DArray*)arg);
    v = zArrayElemNC((zVec3DArray*)arg,i);
  } else{
    n = zPH3DVertNum((zPH3D*)obj);
    v = zPH3DVert((zPH3D*)obj,i);
  }
  zVec3DFromZTK( v, ztk );
  if( zeo_ph_echo_while_reading ){
    i++;
    eprintf( "\r%d/%d vertices", i, n );
    if( i == n ) eprintf( "\n" );
  }
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
  if( zeo_ph_echo_while_reading ){
    i++;
    eprintf( "\r%d/%d facets", i, zPH3DFaceNum((zPH3D*)obj) );
    if( i == zPH3DFaceNum((zPH3D*)obj) ) eprintf( "\n" );
  }
  return obj;
}

static bool _zPH3DLoopArcFromZTK(ZTK *ztk, zVec2DList *vlist)
{
  zVec2D *v1, v2, d1, d2, c, d;
  zDir dir;
  double r, angle, cr;
  int div, n, i;

  dir = zDirFromStr( ZTKVal(ztk) );
  if( dir != ZEO_DIR_CW && dir != ZEO_DIR_CCW ){
    ZRUNERROR( ZEO_ERR_INVALID_DIR_NAME, ZTKVal(ztk) );
    return false;
  }
  ZTKValNext(ztk);
  r = ZTKDouble(ztk);
  if( ( n = div = ZTKInt(ztk) ) == 0 ){
    ZRUNERROR( ZEO_ERR_ZERODIV );
    return false;
  }
  if( ZTKValPtr(ztk) ){
    v2.e[0] = ZTKDouble(ztk);
    v2.e[1] = ZTKDouble(ztk);
  } else{
    zVec2DCopy( &zListTail(vlist)->data, &v2 );
    n--;
  }
  v1 = &zListHead(vlist)->data;
  /* center */
  zVec2DSub( &v2, v1, &d1 );
  zVec2DRot( &d1, dir == ZEO_DIR_CW ? -zPI_2 : zPI_2, &c );
  if( ( cr = zSqr(r) / zVec2DSqrNorm(&d1) - 0.25 ) < 0 ){
    ZRUNERROR( ZEO_ERR_PH_TOOSMALL_ARC, r );
    return false;
  }
  zVec2DMulDRC( &c, sqrt( cr ) );
  zVec2DCatDRC( &c, 0.5, &d1 );
  zVec2DAddDRC( &c, v1 );
  /* radius vector */
  zVec2DSub( v1, &c, &d1 );
  zVec2DSub( &v2, &c, &d2 );
  angle = zVec2DAngle( &d1, &d2 );
  /* vertices list */
  for( i=1; i<=n; i++ ){
    zVec2DRot( &d1, angle * i / div, &d );
    zVec2DAddDRC( &d, &c );
    if( !zVec2DListAdd( vlist, &d ) ) return false;
  }
  return true;
}
static void *_zPH3DLoopFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  zAxis plane_axis, vert1_axis, vert2_axis;
  double plane_val;
  zVec2DList vlist;
  zVec2DListCell *cp;
  zVec2D v;
  int j;

  plane_axis = zAxisFromStr( ZTKVal(ztk) );
  if( plane_axis != zX && plane_axis != zY && plane_axis != zZ ){
    ZRUNERROR( ZEO_ERR_INVALID_AXIS_NAME, ZTKVal(ztk) );
    return NULL;
  }
  if( !ZTKValNext( ztk ) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_LOOP );
    return NULL;
  }
  plane_val = ZTKDouble(ztk);
  vert1_axis = ( plane_axis + 1 ) % 3;
  vert2_axis = ( plane_axis + 2 ) % 3;
  /* loop */
  zListInit( &vlist );
  while( ZTKValPtr(ztk) ){
    if( ZTKValCmp( ztk, "arc" ) ){
      ZTKValNext( ztk );
      if( !_zPH3DLoopArcFromZTK( ztk, &vlist ) ) break;
    } else{
      v.e[0] = ZTKDouble(ztk);
      v.e[1] = ZTKDouble(ztk);
      if( !zVec2DListAdd( &vlist, &v ) ) break;
    }
  }
  /* vertices */
  zArrayAlloc( (zVec3DArray*)arg, zVec3D, zListSize(&vlist) );
  if( zArraySize((zVec3DArray*)arg) != zListSize(&vlist) ) return NULL;
  j = 0;
  zListForEach( &vlist, cp ){
    ((zVec3D*)zArrayElemNC((zVec3DArray*)arg,j))->e[vert1_axis] = cp->data.e[0];
    ((zVec3D*)zArrayElemNC((zVec3DArray*)arg,j))->e[vert2_axis] = cp->data.e[1];
    ((zVec3D*)zArrayElemNC((zVec3DArray*)arg,j))->e[plane_axis] = plane_val;
    j++;
  }
  zVec2DListDestroy( &vlist );
  zeo_ph_echo_while_reading ? eprintf( "created a loop with %d vertices\n", zArraySize((zVec3DArray*)arg) ) : 0;
  return obj;
}

static void *_zPH3DCreatePrismFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  zVec3D shift;

  if( zPH3DVertNum((zPH3D*)obj) > 0 || zPH3DFaceNum((zPH3D*)obj) > 0 ){
    ZRUNWARN( ZEO_WARN_PH_DUPDEF );
  } else{
    zVec3DFromZTK( &shift, ztk );
    obj = zPH3DCreatePrism( (zPH3D*)obj, zArrayBuf((zVec3DArray*)arg), zArraySize((zVec3DArray*)arg), &shift );
    zeo_ph_echo_while_reading ? eprintf( "created a prism\n" ) : 0;
    zArrayFree( (zVec3DArray*)arg );
  }
  return obj;
}

static void *_zPH3DCreatePyramidFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  zVec3D shift;

  if( zPH3DVertNum((zPH3D*)obj) > 0 || zPH3DFaceNum((zPH3D*)obj) > 0 ){
    ZRUNWARN( ZEO_WARN_PH_DUPDEF );
  } else{
    zVec3DFromZTK( &shift, ztk );
    obj = zPH3DCreatePyramid( (zPH3D*)obj, zArrayBuf((zVec3DArray*)arg), zArraySize((zVec3DArray*)arg), &shift );
    zArrayFree( (zVec3DArray*)arg );
    zeo_ph_echo_while_reading ? eprintf( "created a pyramid\n" ) : 0;
  }
  return obj;
}

static const ZTKPrp __ztk_prp_ph[] = {
  { ZTK_KEY_ZEO_PH_VERT,   -1, _zPH3DVertFromZTK,    NULL },
  { ZTK_KEY_ZEO_PH_FACE,   -1, _zPH3DFaceFromZTK,    NULL },
  { ZTK_KEY_ZEO_PH_LOOP,    1, _zPH3DLoopFromZTK,    NULL },
  { ZTK_KEY_ZEO_PH_PRISM,   1, _zPH3DCreatePrismFromZTK,   NULL },
  { ZTK_KEY_ZEO_PH_PYRAMID, 1, _zPH3DCreatePyramidFromZTK, NULL },
};

/* read a 3D polyhedron from a ZTK format processor. */
zPH3D *zPH3DFromZTK(zPH3D *ph, ZTK *ztk)
{
  int num_vert, num_face;
  zVec3DArray varray;

  zPH3DInit( ph );
  zArrayInit( &varray );
  if( !ZTKKeyRewind( ztk ) ) return NULL;
  num_vert = ZTKCountKey( ztk, ZTK_KEY_ZEO_PH_VERT );
  num_face = ZTKCountKey( ztk, ZTK_KEY_ZEO_PH_FACE );
  if( ZTKCountKey( ztk, ZTK_KEY_ZEO_PH_PRISM ) > 0 ||
      ZTKCountKey( ztk, ZTK_KEY_ZEO_PH_PYRAMID ) > 0 ){
    if( num_face > 0 ){
      ZRUNERROR( ZEO_ERR_PH_INVALID_FACE, num_face );
      return NULL;
    }
    if( num_vert > 0 ){
      zArrayAlloc( &varray, zVec3D, num_vert );
      if( zArraySize(&varray) != num_vert ) return NULL;
    }
  } else{
    zArrayAlloc( &ph->vert, zVec3D, num_vert );
    zArrayAlloc( &ph->face, zTri3D, num_face );
    if( zPH3DVertNum(ph) != num_vert ||
        zPH3DFaceNum(ph) != num_face ) return NULL;
  }
  /* vertices & faces */
  ph = (zPH3D *)ZTKEvalKey( ph, &varray, ztk, __ztk_prp_ph );
  if( zArraySize(&varray) != 0 )
    zArrayFree( &varray );
  return ph;
}

/* print a 3D polyhedron to a file. */
void zPH3DFPrintZTK(FILE *fp, const zPH3D *ph)
{
  int i;

  if( !ph || zPH3DVertNum(ph) == 0 ) return;
  for( i=0; i<zPH3DVertNum(ph); i++ ){
    fprintf( fp, "%s: %d ", ZTK_KEY_ZEO_PH_VERT, i );
    zVec3DFPrint( fp, zPH3DVert(ph,i) );
  }
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    fprintf( fp, "%s: %d %d %d\n", ZTK_KEY_ZEO_PH_FACE,
      (int)( zPH3DFaceVert(ph,i,0)-zPH3DVertBuf(ph) ),
      (int)( zPH3DFaceVert(ph,i,1)-zPH3DVertBuf(ph) ),
      (int)( zPH3DFaceVert(ph,i,2)-zPH3DVertBuf(ph) ) );
  }
}
