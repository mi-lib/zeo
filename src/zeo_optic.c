/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_optic - optical properties
 */

#include <zeo/zeo_optic.h>

/* ********************************************************** */
/* CLASS: zOpticalInfo
 * class for the optical characteristic parameter set
 * ********************************************************** */

/* create a set of optical parameters. */
zOpticalInfo *zOpticalInfoCreate(zOpticalInfo *oi, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, double esr, double sns, double alpha, char *name)
{
  zNameSet( oi, name );
  zRGBSet( &oi->amb, ar, ag, ab );
  zRGBSet( &oi->dif, dr, dg, db );
  zRGBSet( &oi->spc, sr, sg, sb );
  oi->esr = esr;
  oi->sns = sns;
  oi->alpha = alpha;
  return oi;
}

/* copy a set of optical parameters. */
zOpticalInfo *zOpticalInfoCopy(zOpticalInfo *src, zOpticalInfo *dest)
{
  zOpticalInfoSetAmb( dest, &src->amb );
  zOpticalInfoSetDif( dest, &src->dif );
  zOpticalInfoSetSpc( dest, &src->spc );
  dest->esr = src->esr;
  dest->sns = src->sns;
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
  zRGBMul( &oi1->amb, &oi2->amb, &oi->amb );
  zRGBMul( &oi1->dif, &oi2->dif, &oi->dif );
  zRGBMul( &oi1->spc, &oi2->spc, &oi->spc );
  oi->esr = oi1->esr * oi2->esr;
  oi->sns = oi1->sns * oi2->sns;
  oi->alpha = oi1->alpha * oi2->alpha;
  return oi;
}

/* blend a pair of sets of optical parameters at a given ratio. */
zOpticalInfo *zOpticalInfoBlend(zOpticalInfo *oi1, zOpticalInfo *oi2, double ratio, zOpticalInfo *oi, char *name)
{
  double rn;

  ratio = _zLimit( ratio, 0, 1 );
  zNameSet( oi, name );
  zRGBBlend( &oi1->amb, &oi2->amb, ratio, &oi->amb );
  zRGBBlend( &oi1->dif, &oi2->dif, ratio, &oi->dif );
  zRGBBlend( &oi1->spc, &oi2->spc, ratio, &oi->spc );
  rn = 1 - ratio;
  oi->esr = ratio*oi1->esr + rn*oi2->esr;
  oi->sns = ratio*oi1->sns + rn*oi2->sns;
  oi->alpha = ratio*oi1->alpha + rn*oi2->alpha;
  return oi;
}

/* parsing a ZTK format. */

static void *_zOpticalInfoNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNameSet( (zOpticalInfo *)obj, ZTKVal(ztk) );
  return zNamePtr((zOpticalInfo *)obj) ? obj : NULL; }
static void *_zOpticalInfoAmbFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->amb, ztk );
  return obj; }
static void *_zOpticalInfoDifFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->dif, ztk );
  return obj; }
static void *_zOpticalInfoSpcFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->spc, ztk );
  return obj; }
static void *_zOpticalInfoEsrFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->esr = ZTKDouble(ztk);
  return obj; }
static void *_zOpticalInfoSnsFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->sns = ZTKDouble(ztk);
  return obj; }
static void *_zOpticalInfoAlphaFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->alpha = ZTKDouble(ztk);
  return obj; }

static void _zOpticalInfoNameFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zOpticalInfo*)obj) ); }
static void _zOpticalInfoAmbFPrintZTK(FILE *fp, int i, void *obj){
  zRGBFPrint( fp, &((zOpticalInfo*)obj)->amb ); }
static void _zOpticalInfoDifFPrintZTK(FILE *fp, int i, void *obj){
  zRGBFPrint( fp, &((zOpticalInfo*)obj)->dif ); }
static void _zOpticalInfoSpcFPrintZTK(FILE *fp, int i, void *obj){
  zRGBFPrint( fp, &((zOpticalInfo*)obj)->spc ); }
static void _zOpticalInfoEsrFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->esr ); }
static void _zOpticalInfoSnsFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->sns ); }
static void _zOpticalInfoAlphaFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->alpha ); }

static ZTKPrp __ztk_prp_optic[] = {
  { "name", 1, _zOpticalInfoNameFromZTK, _zOpticalInfoNameFPrintZTK },
  { "ambient", 1, _zOpticalInfoAmbFromZTK, _zOpticalInfoAmbFPrintZTK },
  { "diffuse", 1, _zOpticalInfoDifFromZTK, _zOpticalInfoDifFPrintZTK },
  { "specular", 1, _zOpticalInfoSpcFromZTK, _zOpticalInfoSpcFPrintZTK },
  { "esr", 1, _zOpticalInfoEsrFromZTK, _zOpticalInfoEsrFPrintZTK },
  { "shininess", 1, _zOpticalInfoSnsFromZTK, _zOpticalInfoSnsFPrintZTK },
  { "alpha", 1, _zOpticalInfoAlphaFromZTK, _zOpticalInfoAlphaFPrintZTK },
};

/* encode an optical info from a ZTK format processor. */
zOpticalInfo *zOpticalInfoFromZTK(zOpticalInfo *oi, ZTK *ztk)
{
  zOpticalInfoInit( oi );
  return ZTKEvalKey( oi, NULL, ztk, __ztk_prp_optic );
}

/* print information of the optical parameter set out to a file. */
void zOpticalInfoFPrintZTK(FILE *fp, zOpticalInfo *oi)
{
  ZTKPrpKeyFPrint( fp, oi, __ztk_prp_optic );
  fprintf( fp, "\n" );
}
