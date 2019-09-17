/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape - 3D shape
 */

#include <zeo/zeo_shape.h>

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
  return shape;
}

/* destroy a 3D shape. */
void zShape3DDestroy(zShape3D *shape)
{
  if( !shape ) return;
  zNameFree( shape );
  shape->com->_destroy( shape->body );
  zFree( shape->body );
  zShape3DSetOptic( shape, NULL );
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
  return cln;
}

/* mirror a 3D shape. */
zShape3D *zShape3DMirror(zShape3D *src, zShape3D *dest, zAxis axis)
{
  if( !( dest->body = ( dest->com = src->com )->_mirror( src->body, axis ) ) )
    return NULL;
  zShape3DSetOptic( dest, zShape3DOptic(src) );
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
    shape->com = &zeo_shape_ph3d_com;
    return shape;
  }
  zPH3DDestroy( ph );
  zFree( ph );
  return NULL;
}

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
  bool mirrored;
} _zShape3DRefPrp;

static void *_zShape3DNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zShape3D *shape;
  zArrayFindName( ((_zShape3DRefPrp*)arg)->sarray, ZTKVal(ztk), shape );
  if( shape ){
    ZRUNWARN( ZEO_WARN_SHAPE_DUP, ZTKVal(ztk) );
    return NULL;
  }
  zNameSet( (zShape3D*)obj, ZTKVal(ztk) );
  return zNamePtr((zShape3D*)obj) ? obj : NULL;
}
static void *_zShape3DTypeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  /* this should be implemented as a more general function. */
  zShape3DCom *com[] = {
    &zeo_shape_ph3d_com, &zeo_shape_box3d_com,
    &zeo_shape_sphere3d_com, &zeo_shape_ellips3d_com,
    &zeo_shape_cyl3d_com, &zeo_shape_ecyl3d_com, &zeo_shape_cone3d_com,
    &zeo_shape_nurbs_com,
    NULL,
  };
  register int k;

  ((zShape3D*)obj)->com = NULL;
  for( k=0; com[k]; k++ )
    if( strcmp( com[k]->typestr, ZTKVal(ztk) ) == 0 ){
      ((zShape3D*)obj)->com = com[k];
      return ( ((zShape3D*)obj)->body = ((zShape3D*)obj)->com->_alloc() ) ? obj : NULL;
    }
  return NULL;
}
static void *_zShape3DOpticFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zArrayFindName( ((_zShape3DRefPrp*)arg)->oarray, ZTKVal(ztk), zShape3DOptic((zShape3D*)obj) );
  if( !zShape3DOptic((zShape3D*)obj) )
    ZRUNWARN( ZEO_WARN_SHAPE_UNKNOWNOPTIC, ZTKVal(ztk) );
  return obj;
}
static void *_zShape3DMirrorFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zShape3D *ref;
  zArrayFindName( ((_zShape3DRefPrp*)arg)->sarray, ZTKVal(ztk), ref );
  if( ref ){
    if( !ZTKValNext(ztk) ) return NULL;
    if( !zShape3DMirror( ref, (zShape3D*)obj, zAxisFromStr(ZTKVal(ztk)) ) ) return NULL;
    ((_zShape3DRefPrp*)arg)->mirrored = true;
  } else
    ZRUNWARN( ZEO_ERR_SHAPE_UNDEF, ZTKVal(ztk) );
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

static ZTKPrp __ztk_prp_shape[] = {
  { "name", 1, _zShape3DNameFromZTK, _zShape3DNameFPrint },
  { "type", 1, _zShape3DTypeFromZTK, _zShape3DTypeFPrint },
  { "optic", 1, _zShape3DOpticFromZTK, _zShape3DOpticFPrint },
  { "mirror", 1, _zShape3DMirrorFromZTK, NULL },
};

/* register a definition of tag-and-keys for a 3D shape to a ZTK format processor. */
bool zShape3DRegZTK(ZTK *ztk)
{
  return ZTKDefRegPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_shape ) &&
         zBox3DDefRegZTK( ztk, ZTK_TAG_SHAPE ) &&
         zSphere3DDefRegZTK( ztk, ZTK_TAG_SHAPE ) &&
         zCyl3DDefRegZTK( ztk, ZTK_TAG_SHAPE ) &&
         zCone3DDefRegZTK( ztk, ZTK_TAG_SHAPE ) &&
         zEllips3DDefRegZTK( ztk, ZTK_TAG_SHAPE ) &&
         zECyl3DDefRegZTK( ztk, ZTK_TAG_SHAPE ) &&
         zPH3DDefRegZTK( ztk, ZTK_TAG_SHAPE ) &&
         zNURBS3DDefRegZTK( ztk, ZTK_TAG_SHAPE );
}

/* read a 3D shape from a ZTK format processor. */
zShape3D *zShape3DFromZTK(zShape3D *shape, zShape3DArray *sarray, zOpticalInfoArray *oarray, ZTK *ztk)
{
  _zShape3DRefPrp prp;

  zShape3DInit( shape );
  /* type, name, associated optical info and mirroring */
  prp.sarray = sarray;
  prp.oarray = oarray;
  prp.mirrored = false;
  if( !ZTKEncodeKey( shape, &prp, ztk, __ztk_prp_shape ) ) return NULL;
  if( prp.mirrored ) return shape;
  if( !shape->com ){
    ZRUNERROR( ZEO_ERR_SHAPE_INVALID );
    return NULL;
  }
  if( !shape->com->_fromZTK( shape->body, ztk ) ) return NULL;
  return shape;
}

/* print out a 3D shape to a file. */
void zShape3DFPrint(FILE *fp, zShape3D *shape)
{
  if( !shape ) return;
  ZTKPrpKeyFPrint( fp, shape, __ztk_prp_shape );
  shape->com->_fprint( fp, shape->body );
  fprintf( fp, "\n" );
}




typedef struct{
  zShape3D *shape;
  zShape3D *sarray;
  int ns;
  zOpticalInfo *oarray;
  int no;
  bool referred;
} _zShape3DParam;

static bool _zShape3DFScan(FILE *fp, void *instance, char *buf, bool *success);

/* (static)
 * scan a 3D shape (internal function). */
bool _zShape3DFScan(FILE *fp, void *instance, char *buf, bool *success)
{
  zShape3DCom *com[] = {
    &zeo_shape_ph3d_com, &zeo_shape_box3d_com,
    &zeo_shape_sphere3d_com, &zeo_shape_ellips3d_com,
    &zeo_shape_cyl3d_com, &zeo_shape_ecyl3d_com, &zeo_shape_cone3d_com,
    &zeo_shape_nurbs_com,
    NULL,
  };
  _zShape3DParam *prm;
  zShape3D *ref = NULL;
  register int k;

  prm = instance;
  if( strcmp( buf, "type" ) == 0 ){
    prm->shape->com = NULL;
    zFToken( fp, buf, BUFSIZ );
    for( k=0; com[k]; k++ )
      if( strcmp( com[k]->typestr, buf ) == 0 )
        prm->shape->com = com[k];
  } else if( strcmp( buf, "name" ) == 0 ){
    if( !( zNameSet( prm->shape, zFToken(fp,buf,BUFSIZ) ) ) )
      return ( *success = false );
  } else if( strcmp( buf, "optic" ) == 0 ){
    zFToken( fp, buf, BUFSIZ );
    zNameFind( prm->oarray, prm->no, buf, zShape3DOptic(prm->shape) );
  } else if( strcmp( buf, "mirror" ) == 0 ){
    zFToken( fp, buf, BUFSIZ );
    zNameFind( prm->sarray, prm->ns, buf, ref );
    if( !ref ){
      ZRUNWARN( ZEO_ERR_SHAPE_UNDEF, buf );
    } else{
      zFToken( fp, buf, BUFSIZ );
      if( !zShape3DMirror( ref, prm->shape, zAxisFromStr(buf) ) )
        return ( *success = false );
      prm->referred = true;
    }
  } else
    return false;
  return true;
}

/* scan a 3D shape from a file. */
zShape3D *zShape3DFScan(FILE *fp, zShape3D *shape, zShape3D *sarray, int ns, zOpticalInfo *oarray, int no)
{
  _zShape3DParam prm;
  int cur;

  prm.shape = shape;
  prm.sarray = sarray;
  prm.ns = ns;
  prm.oarray = oarray;
  prm.no = no;
  prm.referred = false;
  zShape3DInit( shape );
  cur = ftell( fp );
  if( !zFieldFScan( fp, _zShape3DFScan, &prm ) ) goto ERROR;
  if( prm.referred ) return shape;
  if( !zNamePtr( shape ) ){
    ZRUNERROR( ZEO_ERR_SHAPE_UNNAME );
    goto ERROR;
  }
  fseek( fp, cur, SEEK_SET );
  if( !( shape->body = shape->com->_alloc() ) ) goto ERROR;
  if( !shape->com->_fscan( fp, shape->body ) ) goto ERROR;
  return shape;

 ERROR:
  zShape3DDestroy( shape );
  return NULL;
}
