/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d - 3D shape
 */

#include <zeo/zeo_shape3d.h>

/* ********************************************************** */
/* CLASS: zShape3D
 * 3D unit shape class
 * ********************************************************** */

/* initialize a 3D shape. */
zShape3D *zShape3DInit(zShape3D *shape)
{
  zNameSetPtr( shape, NULL );
  shape->body = NULL;
  shape->com = NULL;
  zShape3DSetOptic( shape, NULL );
  zShape3DSetTexture( shape, NULL );
  return shape;
}

/* assign a method of a 3D shape by referring a string. */
zShape3D *zShape3DQueryAssign(zShape3D *shape, char *str)
{
  ZEO_SHAPE_COM_ARRAY;
  register int k;

  shape->com = NULL;
  for( k=0; _zeo_shape_com[k]; k++ )
    if( strcmp( _zeo_shape_com[k]->typestr, str ) == 0 )
      return ( shape->body = ( shape->com = _zeo_shape_com[k] )->_alloc() ) ? shape : NULL;
  return NULL;
}

/* destroy a 3D shape. */
void zShape3DDestroy(zShape3D *shape)
{
  if( !shape ) return;
  zNameFree( shape );
  shape->com->_destroy( shape->body );
  zFree( shape->body );
  zShape3DSetOptic( shape, NULL );
  zShape3DSetTexture( shape, NULL );
}

/* clone a 3D shape. */
zShape3D *zShape3DClone(zShape3D *org, zShape3D *cln, zOpticalInfo *oi)
{
  if( !zNameSet( cln, zName(org) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( cln->body = ( cln->com = org->com )->_clone( org->body ) ) )
    return NULL;
  zShape3DSetOptic( cln, oi );
  zShape3DSetTexture( cln, zShape3DTexture(org) );
  return cln;
}

/* mirror a 3D shape. */
zShape3D *zShape3DMirror(zShape3D *src, zShape3D *dest, zAxis axis)
{
  if( !( dest->body = ( dest->com = src->com )->_mirror( src->body, axis ) ) )
    return NULL;
  zShape3DSetOptic( dest, zShape3DOptic(src) );
  zShape3DSetTexture( dest, zShape3DTexture(src) );
  return dest;
}

/* transform coordinates of a 3D shape. */
zShape3D *zShape3DXform(zShape3D *src, zFrame3D *f, zShape3D *dest)
{
  src->com->_xform( src->body, f, dest->body );
  return dest;
}

/* inversely transform coordinates of a 3D shape. */
zShape3D *zShape3DXformInv(zShape3D *src, zFrame3D *f, zShape3D *dest)
{
  src->com->_xforminv( src->body, f, dest->body );
  return dest;
}

/* closest point to a 3D shape. */
double zShape3DClosest(zShape3D *shape, zVec3D *p, zVec3D *cp)
{
  return shape->com->_closest( shape->body, p, cp );
}

/* distance from a point to a 3D shape. */
double zShape3DPointDist(zShape3D *shape, zVec3D *p)
{
  return shape->com->_pointdist( shape->body, p );
}

/* check if a point is inside of a 3D shape. */
bool zShape3DPointIsInside(zShape3D *shape, zVec3D *p, bool rim)
{
  return shape->com->_pointisinside( shape->body, p, rim );
}

/* volume of a 3D shape. */
double zShape3DVolume(zShape3D *shape)
{
  return shape->com->_volume( shape->body );
}

/* barycenter of a 3D shape. */
zVec3D *zShape3DBarycenter(zShape3D *shape, zVec3D *c)
{
  return shape->com->_barycenter( shape->body, c );
}

/* inertia tensor about barycenter of a 3D shape from mass. */
zMat3D *zShape3DBaryInertiaMass(zShape3D *shape, double mass, zMat3D *inertia)
{
  return shape->com->_baryinertia_m( shape->body, mass, inertia );
}

/* inertia tensor about barycenter of a 3D shape. */
zMat3D *zShape3DBaryInertia(zShape3D *shape, double density, zMat3D *inertia)
{
  return shape->com->_baryinertia_d( shape->body, density, inertia );
}

/* inertia tensor about origin of a 3D shape from mass. */
zMat3D *zShape3DInertiaMass(zShape3D *shape, double mass, zMat3D *inertia)
{
  zVec3D c;

  zShape3DBarycenter( shape, &c );
  zShape3DBaryInertiaMass( shape, mass, inertia );
  return zMat3DCatVec3DDoubleOuterProdDRC( inertia, -mass, &c );
}

/* inertia tensor about origin of a 3D shape. */
zMat3D *zShape3DInertia(zShape3D *shape, double density, zMat3D *inertia)
{
  zVec3D c;
  double mass;

  zShape3DBarycenter( shape, &c );
  mass = density * zShape3DVolume( shape );
  zShape3DBaryInertiaMass( shape, mass, inertia );
  return zMat3DCatVec3DDoubleOuterProdDRC( inertia, -mass, &c );
}

/* convert a shape to a polyhedron. */
zShape3D *zShape3DToPH(zShape3D *shape)
{
  zPH3D *ph;

  if( strcmp( shape->com->typestr, "polyhedron" ) == 0 ) return shape;
  if( !( ph = zAlloc( zPH3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( shape->com->_toph( shape->body, ph ) ){
    shape->body = ph;
    shape->com = &zeo_shape3d_ph_com;
    return shape;
  }
  zPH3DDestroy( ph );
  zFree( ph );
  return NULL;
}

/* assign methods for polyhedron class to a shape. */
static void _zShape3DAssignPH(zShape3D *shape)
{
  if( shape->com && shape->com != &zeo_shape3d_ph_com )
    ZRUNWARN( ZEO_WARN_SHAPE_OVRRDN_PH );
  zShape3DQueryAssign( shape, "polyhedron" );
}

/* read a shape from a STL file. */
zShape3D *zShape3DFReadSTL(FILE *fp, zShape3D *shape)
{
  char buf[BUFSIZ];

  _zShape3DAssignPH( shape );
  if( !zPH3DFReadSTL( fp, zShape3DPH(shape), buf, BUFSIZ ) ) return NULL;
  if( !zNamePtr(shape) )
    if( !zNameSet( shape, buf ) ) return NULL;
  return shape;
}

/* read a shape from a PLY file. */
zShape3D *zShape3DFReadPLY(FILE *fp, zShape3D *shape)
{
  _zShape3DAssignPH( shape );
  if( !zPH3DFReadPLY( fp, zShape3DPH(shape) ) ) return NULL;
  return shape;
}

/* read a shape from a OBJ file. */
zShape3D *zShape3DFReadOBJ(FILE *fp, zShape3D *shape)
{
  _zShape3DAssignPH( shape );
  if( !zPH3DFReadOBJ( fp, zShape3DPH(shape) ) ) return NULL;
  return shape;
}

#ifdef __ZEO_USE_DAE
/* read a shape from a DAE file. */
zShape3D *zShape3DFReadDAE(zShape3D *shape, char *filename)
{
  _zShape3DAssignPH( shape );
  if( !zPH3DFReadDAE( zShape3DPH(shape), filename ) ) return NULL;
  return shape;
}
#endif /* __ZEO_USE_DAE */

/* parse ZTK format */

/* read the number of division for smooth primitives from a ZTK format processor. */
int zShape3DDivFromZTK(ZTK *ztk)
{
  int val;
  return ( val = ZTKInt(ztk) ) > 0 ? val : ZEO_SHAPE_DEFAULT_DIV;
}

typedef struct{
  zShape3DArray *sarray;
  zOpticalInfoArray *oarray;
  zTextureArray *tarray;
  bool imported;
  zFrame3D f;
  bool xformed;
} _zShape3DRefPrp;

static void *_zShape3DNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zShape3D *shape = NULL;
  if( ((_zShape3DRefPrp*)arg)->sarray )
    zArrayFindName( ((_zShape3DRefPrp*)arg)->sarray, ZTKVal(ztk), shape );
  if( shape ){
    ZRUNWARN( ZEO_WARN_SHAPE_DUP, ZTKVal(ztk) );
    return NULL;
  }
  zNameSet( (zShape3D*)obj, ZTKVal(ztk) );
  return zNamePtr((zShape3D*)obj) ? obj : NULL;
}
static void *_zShape3DTypeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zShape3DQueryAssign( (zShape3D*)obj, ZTKVal(ztk) );
}
static void *_zShape3DOpticFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  if( ((_zShape3DRefPrp*)arg)->oarray )
    zArrayFindName( ((_zShape3DRefPrp*)arg)->oarray, ZTKVal(ztk), zShape3DOptic((zShape3D*)obj) );
  if( !zShape3DOptic((zShape3D*)obj) )
    ZRUNWARN( ZEO_WARN_SHAPE_UNKNOWNOPTIC, ZTKVal(ztk) );
  return obj;
}
static void *_zShape3DTextureFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  if( ((_zShape3DRefPrp*)arg)->tarray )
    zArrayFindName( ((_zShape3DRefPrp*)arg)->tarray, ZTKVal(ztk), zShape3DTexture((zShape3D*)obj) );
  if( !zShape3DTexture((zShape3D*)obj) )
    ZRUNWARN( ZEO_WARN_SHAPE_UNKNOWNTEXTURE, ZTKVal(ztk) );
  return obj;
}
static void *_zShape3DMirrorFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zShape3D *ref = NULL;
  if( ((_zShape3DRefPrp*)arg)->sarray )
    zArrayFindName( ((_zShape3DRefPrp*)arg)->sarray, ZTKVal(ztk), ref );
  if( ref ){
    if( !ZTKValNext(ztk) ) return NULL;
    if( !zShape3DMirror( ref, (zShape3D*)obj, zAxisFromStr(ZTKVal(ztk)) ) ) return NULL;
    ((_zShape3DRefPrp*)arg)->imported = true;
  } else
    ZRUNWARN( ZEO_ERR_SHAPE_UNDEF, ZTKVal(ztk) );
  return obj;
}
static void *_zShape3DImportFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  char *suffix;
  FILE *fp;

  suffix = zGetSuffix( ZTKVal(ztk) );
  if( strcmp( suffix, "dae" ) == 0 || strcmp( suffix, "DAE" ) == 0 ){
#ifdef __ZEO_USE_DAE
    if( !zShape3DFReadDAE( obj, ZTKVal(ztk) ) ) obj = NULL;
#else
    ZRUNWARN( ZEO_ERR_DAE_UNSUPPORTED );
    obj = NULL;
#endif
  } else{
    if( !( fp = fopen( ZTKVal(ztk), "r" ) ) ){
      ZOPENERROR( ZTKVal(ztk) );
      return NULL;
    }
    if( strcmp( suffix, "stl" ) == 0 || strcmp( suffix, "STL" ) == 0 ){
      if( !zShape3DFReadSTL( fp, obj ) ) obj = NULL;
    } else
    if( strcmp( suffix, "ply" ) == 0 || strcmp( suffix, "PLY" ) == 0 ){
      if( !zShape3DFReadPLY( fp, obj ) ) obj = NULL;
    } else
    if( strcmp( suffix, "obj" ) == 0 || strcmp( suffix, "OBJ" ) == 0 ){
      if( !zShape3DFReadOBJ( fp, obj ) ) obj = NULL;
    } else{
      ZRUNERROR( ZEO_WARN_SHAPE_UNKNOWNFORMAT, suffix );
      obj = NULL;
    }
    fclose( fp );
  }
  if( obj ){
    ((_zShape3DRefPrp*)arg)->imported = true;
    if( ZTKValNext(ztk) )
      zPH3DScale( zShape3DPH((zShape3D*)obj), atof(ZTKVal(ztk)) );
  }
  return obj;
}

static void *_zShape3DPosFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((_zShape3DRefPrp*)arg)->xformed = true;
  zVec3DFromZTK( zFrame3DPos(&((_zShape3DRefPrp*)arg)->f), ztk );
  return obj;
}
static void *_zShape3DAttFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((_zShape3DRefPrp*)arg)->xformed = true;
  zMat3DFromZTK( zFrame3DAtt(&((_zShape3DRefPrp*)arg)->f), ztk );
  return obj;
}
static void *_zShape3DRotFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3D aa;
  ((_zShape3DRefPrp*)arg)->xformed = true;
  zAAFromZTK( &aa, ztk );
  zMat3DRotDRC( zFrame3DAtt(&((_zShape3DRefPrp*)arg)->f), &aa );
  return obj;
}
static void *_zShape3DFrameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((_zShape3DRefPrp*)arg)->xformed = true;
  zFrame3DFromZTK( &((_zShape3DRefPrp*)arg)->f, ztk );
  return obj;
}

static void _zShape3DNameFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zShape3D*)obj) );
}
static void _zShape3DTypeFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", ((zShape3D*)obj)->com->typestr );
}
static void _zShape3DOpticFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName(zShape3DOptic((zShape3D*)obj)) );
}
static void _zShape3DTextureFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName(zShape3DTexture((zShape3D*)obj)) );
}

static ZTKPrp __ztk_prp_shape[] = {
  { "name", 1, _zShape3DNameFromZTK, _zShape3DNameFPrint },
  { "type", 1, _zShape3DTypeFromZTK, _zShape3DTypeFPrint },
  { "optic", 1, _zShape3DOpticFromZTK, _zShape3DOpticFPrint },
  { "texture", 1, _zShape3DTextureFromZTK, _zShape3DTextureFPrint },
  { "mirror", 1, _zShape3DMirrorFromZTK, NULL },
  { "import", 1, _zShape3DImportFromZTK, NULL },
  { "pos", 1, _zShape3DPosFromZTK, NULL },
  { "att", 1, _zShape3DAttFromZTK, NULL },
  { "rot", -1, _zShape3DRotFromZTK, NULL },
  { "frame", 1, _zShape3DFrameFromZTK, NULL },
};

/* read a 3D shape from a ZTK format processor. */
zShape3D *zShape3DFromZTK(zShape3D *shape, zShape3DArray *sarray, zOpticalInfoArray *oarray, zTextureArray *tarray, ZTK *ztk)
{
  _zShape3DRefPrp prp;

  zShape3DInit( shape );
  /* type, name, associated optical info, and mirroring/importing operations */
  prp.sarray = sarray;
  prp.oarray = oarray;
  prp.tarray = tarray;
  prp.imported = false;
  zFrame3DIdent( &prp.f );
  prp.xformed = false;
  if( !ZTKEvalKey( shape, &prp, ztk, __ztk_prp_shape ) ) return NULL;
  if( !prp.imported ){
    if( !shape->com ){
      ZRUNERROR( ZEO_ERR_SHAPE_INVALID );
      return NULL;
    }
    if( !shape->com->_fromZTK( shape->body, ztk ) ) return NULL;
  }
  if( prp.xformed ){
    shape->com->_xform( shape->body, &prp.f, shape->body );
  }
  return shape;
}

/* print out a 3D shape to a file. */
void zShape3DFPrintZTK(FILE *fp, zShape3D *shape)
{
  if( !shape ) return;
  ZTKPrpKeyFPrint( fp, shape, __ztk_prp_shape );
  shape->com->_fprintZTK( fp, shape->body );
  fprintf( fp, "\n" );
}

/* read a 3D shape from a ZTK format file. */
zShape3D *zShape3DReadZTK(zShape3D *shape, char filename[])
{
  ZTK ztk;

  ZTKInit( &ztk );
  ZTKParse( &ztk, filename );
  shape = zShape3DFromZTK( shape, NULL, NULL, NULL, &ztk );
  ZTKDestroy( &ztk );
  return shape;
}

/* write a 3D shape to a ZTK format file. */
bool zShape3DWriteZTK(zShape3D *shape, char filename[])
{
  FILE *fp;

  if( !( fp = zOpenZTKFile( filename, "w" ) ) ){
    ZOPENERROR( filename );
    return false;
  }
  zShape3DFPrintZTK( fp, shape );
  fclose( fp );
  return true;
}
