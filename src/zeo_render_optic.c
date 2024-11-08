/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_render_optic - rendering: optical properties
 */

#include <zeo/zeo_render_optic.h>

/* ********************************************************** */
/* CLASS: zOpticalInfo
 * class for the optical characteristic parameter set
 * ********************************************************** */

/* create a set of optical parameters. */
zOpticalInfo *zOpticalInfoCreate(zOpticalInfo *oi, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, double esr, double shininess, double alpha, char *name)
{
  zNameSet( oi, name );
  zRGBSet( &oi->ambient, ar, ag, ab );
  zRGBSet( &oi->diffuse, dr, dg, db );
  zRGBSet( &oi->specular, sr, sg, sb );
  oi->esr = esr;
  oi->shininess = shininess;
  oi->alpha = alpha;
  return oi;
}

/* copy a set of optical parameters. */
zOpticalInfo *zOpticalInfoCopy(zOpticalInfo *src, zOpticalInfo *dest)
{
  zRGBCopy( &src->ambient, &dest->ambient );
  zRGBCopy( &src->diffuse, &dest->diffuse );
  zRGBCopy( &src->specular, &dest->specular );
  dest->esr = src->esr;
  dest->shininess = src->shininess;
  dest->alpha = src->alpha;
  return dest;
}

/* clone a set of optical parameters. */
zOpticalInfo *zOpticalInfoClone(zOpticalInfo *src, zOpticalInfo *dest)
{
  return zNameSet( dest, zName(src) ) ?
    zOpticalInfoCopy( src, dest ) : NULL;
}

/* multiply a set of optical parameters to another. */
zOpticalInfo *zOpticalInfoMul(zOpticalInfo *oi1, zOpticalInfo *oi2, zOpticalInfo *oi)
{
  zRGBMul( &oi1->ambient, &oi2->ambient, &oi->ambient );
  zRGBMul( &oi1->diffuse, &oi2->diffuse, &oi->diffuse );
  zRGBMul( &oi1->specular, &oi2->specular, &oi->specular );
  oi->esr = oi1->esr * oi2->esr;
  oi->shininess = oi1->shininess * oi2->shininess;
  oi->alpha = oi1->alpha * oi2->alpha;
  return oi;
}

/* blend a pair of sets of optical parameters at a given ratio. */
zOpticalInfo *zOpticalInfoBlend(zOpticalInfo *oi1, zOpticalInfo *oi2, double ratio, zOpticalInfo *oi, char *name)
{
  double rn;

  ratio = _zLimit( ratio, 0, 1 );
  zNameSet( oi, name );
  zRGBBlend( &oi1->ambient, &oi2->ambient, ratio, &oi->ambient );
  zRGBBlend( &oi1->diffuse, &oi2->diffuse, ratio, &oi->diffuse );
  zRGBBlend( &oi1->specular, &oi2->specular, ratio, &oi->specular );
  rn = 1 - ratio;
  oi->esr = ratio*oi1->esr + rn*oi2->esr;
  oi->shininess = ratio*oi1->shininess + rn*oi2->shininess;
  oi->alpha = ratio*oi1->alpha + rn*oi2->alpha;
  return oi;
}

/* parsing a ZTK format. */

static void *_zOpticalInfoNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNameSet( (zOpticalInfo *)obj, ZTKVal(ztk) );
  return zNamePtr((zOpticalInfo *)obj) ? obj : NULL; }
static void *_zOpticalInfoAmbientFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->ambient, ztk );
  return obj; }
static void *_zOpticalInfoDiffuseFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->diffuse, ztk );
  return obj; }
static void *_zOpticalInfoSpecularFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->specular, ztk );
  return obj; }
static void *_zOpticalInfoESRFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->esr = ZTKDouble(ztk);
  return obj; }
static void *_zOpticalInfoShininessFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->shininess = ZTKDouble(ztk);
  return obj; }
static void *_zOpticalInfoAlphaFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->alpha = ZTKDouble(ztk);
  return obj; }

static bool _zOpticalInfoNameFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zOpticalInfo*)obj) );
  return true; }
static bool _zOpticalInfoAmbientFPrintZTK(FILE *fp, int i, void *obj){
  if( zRGBIsZero( &((zOpticalInfo*)obj)->ambient ) ) return false;
  zRGBFPrint( fp, &((zOpticalInfo*)obj)->ambient );
  return true; }
static bool _zOpticalInfoDiffuseFPrintZTK(FILE *fp, int i, void *obj){
  if( zRGBIsZero( &((zOpticalInfo*)obj)->diffuse ) ) return false;
  zRGBFPrint( fp, &((zOpticalInfo*)obj)->diffuse );
  return true; }
static bool _zOpticalInfoSpecularFPrintZTK(FILE *fp, int i, void *obj){
  if( zRGBIsZero( &((zOpticalInfo*)obj)->specular ) ) return false;
  zRGBFPrint( fp, &((zOpticalInfo*)obj)->specular );
  return true; }
static bool _zOpticalInfoESRFPrintZTK(FILE *fp, int i, void *obj){
  if( zIsTiny( ((zOpticalInfo*)obj)->esr ) ) return false;
  fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->esr );
  return true; }
static bool _zOpticalInfoShininessFPrintZTK(FILE *fp, int i, void *obj){
  if( zIsTiny( ((zOpticalInfo*)obj)->shininess ) ) return false;
  fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->shininess );
  return true; }
static bool _zOpticalInfoAlphaFPrintZTK(FILE *fp, int i, void *obj){
  if( zIsTiny( 1.0 - ((zOpticalInfo*)obj)->alpha ) ) return false;
  fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->alpha );
  return true; }

static ZTKPrp __ztk_prp_optic[] = {
  { "name", 1, _zOpticalInfoNameFromZTK, _zOpticalInfoNameFPrintZTK },
  { "ambient", 1, _zOpticalInfoAmbientFromZTK, _zOpticalInfoAmbientFPrintZTK },
  { "diffuse", 1, _zOpticalInfoDiffuseFromZTK, _zOpticalInfoDiffuseFPrintZTK },
  { "specular", 1, _zOpticalInfoSpecularFromZTK, _zOpticalInfoSpecularFPrintZTK },
  { "esr", 1, _zOpticalInfoESRFromZTK, _zOpticalInfoESRFPrintZTK },
  { "shininess", 1, _zOpticalInfoShininessFromZTK, _zOpticalInfoShininessFPrintZTK },
  { "alpha", 1, _zOpticalInfoAlphaFromZTK, _zOpticalInfoAlphaFPrintZTK },
};

/* read an optical info from a ZTK format processor. */
zOpticalInfo *zOpticalInfoFromZTK(zOpticalInfo *oi, ZTK *ztk)
{
  zOpticalInfoInit( oi );
  return (zOpticalInfo *)ZTKEvalKey( oi, NULL, ztk, __ztk_prp_optic );
}

/* add an optical info to a ZTK format processor. */
ZTK *zOpticalInfoToZTK(zOpticalInfo *oi, ZTK *ztk)
{
  if( !ZTKAddKey( ztk, "name" ) ) return NULL;
  if( !ZTKAddVal( ztk, zName(oi) ) ) return NULL;
  if( !zRGBIsZero( &oi->ambient ) ){
    if( !ZTKAddKey( ztk, "ambient" ) ) return NULL;
    if( !zRGBToZTK( &oi->ambient, ztk ) ) return NULL;
  }
  if( !zRGBIsZero( &oi->diffuse ) ){
    if( !ZTKAddKey( ztk, "diffuse" ) ) return NULL;
    if( !zRGBToZTK( &oi->diffuse, ztk ) ) return NULL;
  }
  if( !zRGBIsZero( &oi->specular ) ){
    if( !ZTKAddKey( ztk, "specular" ) ) return NULL;
    if( !zRGBToZTK( &oi->specular, ztk ) ) return NULL;
  }
  if( !zIsTiny( oi->esr ) ){
    if( !ZTKAddKey( ztk, "esr" ) ) return NULL;
    if( !ZTKAddDouble( ztk, oi->esr ) ) return NULL;
  }
  if( !zIsTiny( oi->shininess ) ){
    if( !ZTKAddKey( ztk, "shininess" ) ) return NULL;
    if( !ZTKAddDouble( ztk, oi->shininess ) ) return NULL;
  }
  if( !zIsTiny( oi->alpha ) ){
    if( !ZTKAddKey( ztk, "alpha" ) ) return NULL;
    if( !ZTKAddDouble( ztk, oi->alpha ) ) return NULL;
  }
  return ztk;
}

/* print information of the optical parameter set out to a file. */
void zOpticalInfoFPrintZTK(FILE *fp, zOpticalInfo *oi)
{
  ZTKPrpKeyFPrint( fp, oi, __ztk_prp_optic );
  fprintf( fp, "\n" );
}
