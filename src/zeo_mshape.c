/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mshape - multiple 3D shape
 */

#include <zeo/zeo_mshape.h>

/* ********************************************************** */
/* CLASS: zMShape3D
 * multiple 3D shape class
 * ********************************************************** */

static bool _zMShape3DFRead(FILE *fp, void *instance, char *buf, bool *success);
static zMShape3D *_zMShape3DOpticFAlloc(FILE *fp, zMShape3D *ms);
static zMShape3D *_zMShape3DShapeFAlloc(FILE *fp, zMShape3D *ms);
static zMShape3D *_zMShape3DOpticFRead(FILE *fp, zMShape3D *ms, int i);
static zMShape3D *_zMShape3DShapeFRead(FILE *fp, zMShape3D *ms, int i);

/* zMShape3DInit
 * - initialize 3D multiple shapes.
 */
zMShape3D *zMShape3DInit(zMShape3D *ms)
{
  zArrayInit( &ms->shape );
  zArrayInit( &ms->optic );
  return ms;
}

/* zMShape3DDestroy
 * - destroy 3D multiple shapes.
 */
void zMShape3DDestroy(zMShape3D *ms)
{
  register int i;

  if( !ms ) return;
  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    zShape3DDestroy( zMShape3DShape(ms,i) );
  zArrayFree( &ms->shape );
  for( i=0; i<zMShape3DOpticNum(ms); i++ )
    zOpticalInfoDestroy( zMShape3DOptic(ms,i) );
  zArrayFree( &ms->optic );
}

/* zMShape3DClone
 * - clone 3D multiple shapes.
 */
zMShape3D *zMShape3DClone(zMShape3D *org)
{
  zMShape3D *cln;
  int i;

  if( !( cln = zAlloc( zMShape3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  /* optical info. array */
  zArrayAlloc( &cln->optic, zOpticalInfo, zMShape3DOpticNum(org) );
  if( zMShape3DOpticNum(cln) != zMShape3DOpticNum(org) ) return NULL;
  for( i=0; i<zMShape3DOpticNum(cln); i++ )
    if( !zOpticalInfoClone( zMShape3DOptic(org,i), zMShape3DOptic(cln,i) ) )
      return NULL;
  /* shape array */
  zArrayAlloc( &cln->shape, zShape3D, zMShape3DShapeNum(org) );
  if( zMShape3DShapeNum(cln) != zMShape3DShapeNum(org) ) return NULL;
  for( i=0; i<zMShape3DShapeNum(cln); i++ )
    if( !zShape3DClone( zMShape3DShape(org,i), zMShape3DShape(cln,i),
          zMShape3DOptic( cln, zShape3DOptic(zMShape3DShape(org,i)) - zMShape3DOpticBuf(org) ) ) )
      return NULL;
  return cln;
}

/* zMShape3DContigVert
 * - contiguous vertix of multiple shapes to a point.
 */
zVec3D *zMShape3DContigVert(zMShape3D *ms, zVec3D *p, double *d)
{
  register int i;
  zVec3D *v, *nv;
  double _d, dmin;

  if( !d ) d = &_d;
  v = zShape3DContigVert( zMShape3DShape(ms,0), p, d );
  dmin = *d;
  for( i=1; i<zMShape3DShapeNum(ms); i++ ){
    nv = zShape3DContigVert( zMShape3DShape(ms,i), p, d );
    if( *d < dmin ){
      v = nv;
      dmin = *d;
    }
  }
  *d = dmin;
  return v;
}

/* zMShape3DClosest
 * - the closest point to multiple shapes.
 */
double zMShape3DClosest(zMShape3D *ms, zVec3D *p, zVec3D *cp)
{
  register int i;
  zVec3D ncp;
  double d, dmin;

  dmin = zShape3DClosest( zMShape3DShape(ms,0), p, cp );
  for( i=1; i<zMShape3DShapeNum(ms); i++ )
    if( ( d = zShape3DClosest( zMShape3DShape(ms,i), p, &ncp ) ) < dmin ){
      zVec3DCopy( &ncp, cp );
      dmin = d;
    }
  return dmin;
}

/* zMShape3DPointIsInside
 * - check if a point is inside of multiple shapes.
 */
bool zMShape3DPointIsInside(zMShape3D *ms, zVec3D *p, bool rim)
{
  register int i;

  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    if( zShape3DPointIsInside( zMShape3DShape(ms,i), p, rim ) )
      return true;
  return false;
}

/* zMShape3DToPH
 * - convert all shapes of a multishape to polyhedra.
 */
zMShape3D *zMShape3DToPH(zMShape3D *ms)
{
  register int i;

  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    if( !zShape3DToPH( zMShape3DShape(ms,i) ) ) return NULL;
  return ms;
}

/* zMShape3DReadFile
 * - input of multiple 3D shapes from file refered by name.
 */
zMShape3D *zMShape3DReadFile(zMShape3D *ms, char filename[])
{
  FILE *fp;
  zMShape3D *ret;

  if( !( fp = zOpenFile( filename, ZMULTISHAPE3D_SUFFIX, "r" ) ) )
    return NULL;
  ret = zMShape3DFRead( fp, ms );
  fclose( fp );
  return ret;
}

typedef struct{
  zMShape3D *ms;
  int oc;
  int sc;
} _zMShape3DParam;

/* (static)
 * _zMShape3DFRead
 * input of multiple 3D shapes from file.
 */
bool _zMShape3DFRead(FILE *fp, void *instance, char *buf, bool *success)
{
  _zMShape3DParam *prm;

  prm = instance;
  if( strcmp( buf, ZOPTIC_TAG ) == 0 ){
    if( prm->oc >= zMShape3DOpticNum(prm->ms) ) return false;
    if( !_zMShape3DOpticFRead( fp, prm->ms, prm->oc++ ) )
      return ( *success = false );
  } else if( strcmp( buf, ZSHAPE_TAG ) == 0 ){
    if( prm->sc >= zMShape3DShapeNum(prm->ms) ) return false;
    if( !_zMShape3DShapeFRead( fp, prm->ms, prm->sc++ ) )
      return ( *success = false );
  } else
    return false;
  return true;
}

/* zMShape3DFRead
 * input of multiple 3D shapes from file.
 */
zMShape3D *zMShape3DFRead(FILE *fp, zMShape3D *ms)
{
  _zMShape3DParam prm;

  zMShape3DInit( ms );
  if( _zMShape3DOpticFAlloc(fp,ms) && _zMShape3DShapeFAlloc(fp,ms) ){
    prm.ms = ms;
    prm.sc = 0;
    prm.oc = 0;
    if( zTagFRead( fp, _zMShape3DFRead, &prm ) ) return ms;
  }
  zMShape3DDestroy( ms );
  return NULL;
}

/* (static)
 * _zMShape3DOpticFAlloc
 * - preparation of buffer for optical information sets
 *   for multiple 3D shapes.
 */
zMShape3D *_zMShape3DOpticFAlloc(FILE *fp, zMShape3D *ms)
{
  register int i;
  int n;

  n = zFCountTag( fp, ZOPTIC_TAG );
  zArrayAlloc( &ms->optic, zOpticalInfo, n );
  if( n > 0 && !zMShape3DOpticBuf(ms) ) return NULL;
  for( i=0; i<n; i++ )
    zOpticalInfoInit( zArrayElem(&ms->optic,i) );
  return ms;
}

/* (static)
 * _zMShape3DShapeFAlloc
 * - preparation of buffer for 3D shapes for multiple 3D shapes.
 */
zMShape3D *_zMShape3DShapeFAlloc(FILE *fp, zMShape3D *ms)
{
  register int i;
  int n;

  n = zFCountTag( fp, ZSHAPE_TAG );
  zArrayAlloc( &ms->shape, zShape3D, n );
  if( n > 0 && !zMShape3DShapeBuf(ms) ) return NULL;
  for( i=0; i<n; i++ )
    zShape3DInit( zArrayElem(&ms->shape,i) );
  return ms;
}

/* (static)
 * _zMShape3DOpticFRead
 * - input of optical information for multiple 3D shapes.
 */
zMShape3D *_zMShape3DOpticFRead(FILE *fp, zMShape3D *ms, int i)
{
  if( i >= zMShape3DOpticNum(ms) ){
    ZRUNERROR( ZEO_ERR_OPT_MANY );
    return NULL;
  }
  if( !zOpticalInfoFRead( fp, zMShape3DOptic(ms,i) ) ){
    ZRUNERROR( ZEO_ERR_OPT_INV );
    return NULL;
  }
  if( !zNamePtr( zMShape3DOptic(ms,i) ) ){
    ZRUNERROR( ZEO_ERR_OPT_UNNAME );
    return NULL;
  }
  return ms;
}

/* (static)
 * _zMShape3DShapeFRead
 * - input of 3D shape for multiple 3D shapes.
 */
zMShape3D *_zMShape3DShapeFRead(FILE *fp, zMShape3D *ms, int i)
{
  if( i >= zMShape3DShapeNum(ms) ){
    ZRUNERROR( ZEO_ERR_SHAPE_MANY );
    return NULL;
  }
  if( !zShape3DFRead( fp, zMShape3DShape(ms,i),
    zMShape3DShapeBuf(ms), zMShape3DShapeNum(ms),
    zMShape3DOpticBuf(ms), zMShape3DOpticNum(ms) ) ){
    ZRUNERROR( ZEO_ERR_SHAPE_INV );
    return NULL;
  }
  if( !zNamePtr( zMShape3DShape(ms,i) ) ){
    ZRUNERROR( ZEO_ERR_SHAPE_UNNAME );
    return NULL;
  }
  return ms;
}

/* zMShape3DWriteFile
 * - output of multiple 3D shape to file refered by name.
 */
bool zMShape3DWriteFile(zMShape3D *ms, char filename[])
{
  char fname[BUFSIZ];
  FILE *fp;

  zAddSuffix( filename, ZMULTISHAPE3D_SUFFIX, fname, BUFSIZ );
  if( !( fp = fopen( fname, "w" ) ) ){
    ZOPENERROR( fname );
    return false;
  }
  zMShape3DFWrite( fp, ms );
  fclose( fp );
  return true;
}

/* zMShape3DFWrite
 * - output of multiple 3D shape to file.
 */
void zMShape3DFWrite(FILE *fp, zMShape3D *ms)
{
  register int i;

  for( i=0; i<zMShape3DOpticNum(ms); i++ ){
    fprintf( fp, "[%s]\n", ZOPTIC_TAG );
    zOpticalInfoFWrite( fp, zMShape3DOptic( ms, i ) );
  }
  for( i=0; i<zMShape3DShapeNum(ms); i++ ){
    fprintf( fp, "[%s]\n", ZSHAPE_TAG );
    zShape3DFWrite( fp, zMShape3DShape( ms, i ) );
  }
}
