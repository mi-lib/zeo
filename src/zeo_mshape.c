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

/* initialize multiple shapes. */
zMShape3D *zMShape3DInit(zMShape3D *ms)
{
  zArrayInit( &ms->shape );
  zArrayInit( &ms->optic );
  return ms;
}

/* destroy multiple shapes. */
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

/* clone multiple shapes. */
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

/* find contiguous vertex of multiple shapes to a point. */
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

/* find the closest point on multiple shapes from a point. */
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

/* check if a point is inside of multiple shapes. */
bool zMShape3DPointIsInside(zMShape3D *ms, zVec3D *p, bool rim)
{
  register int i;

  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    if( zShape3DPointIsInside( zMShape3DShape(ms,i), p, rim ) )
      return true;
  return false;
}

/* convert multiple shapes to polyhedra. */
zMShape3D *zMShape3DToPH(zMShape3D *ms)
{
  register int i;

  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    if( !zShape3DToPH( zMShape3DShape(ms,i) ) ) return NULL;
  return ms;
}

/* read informationi of multiple shapes from file. */
zMShape3D *zMShape3DReadFile(zMShape3D *ms, char filename[])
{
  FILE *fp;
  zMShape3D *ret;

  if( !( fp = zOpenZTKFile( filename, "r" ) ) )
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

/* read information of multiple shapes from file (internal operation). */
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

/* read information of multiple shapes from file. */
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

/* allocate memory for optical information of multiple shapes. */
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

/* allocate memory for shapes of multiple shapes. */
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

/* read optical information of multiple shapes. */
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

/* read information of a shape of multiple shapes. */
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

/* output information of multiple shapes to file. */
bool zMShape3DWriteFile(zMShape3D *ms, char filename[])
{
  char fname[BUFSIZ];
  FILE *fp;

  if( !( fp = zOpenZTKFile( fname, "w" ) ) ){
    ZOPENERROR( fname );
    return false;
  }
  zMShape3DFWrite( fp, ms );
  fclose( fp );
  return true;
}

/* output information of multiple shapes to file. */
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
