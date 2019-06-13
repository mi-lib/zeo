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

static bool _zOpticalInfoFScan(FILE *fp, void *instance, char *buf, bool *success);

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

  ratio = zLimit( ratio, 0, 1 );
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

/* (static)
 * scan information of the optical parameter set from a stream. */
bool _zOpticalInfoFScan(FILE *fp, void *instance, char *buf, bool *success)
{
  if( strcmp( buf, "name" ) == 0 )
    zNameSet( (zOpticalInfo *)instance, zFToken( fp, buf, BUFSIZ ) );
  else if( strcmp( buf, "ambient" ) == 0 )
    zRGBFScan( fp, &((zOpticalInfo *)instance)->amb );
  else if( strcmp( buf, "diffuse" ) == 0 )
    zRGBFScan( fp, &((zOpticalInfo *)instance)->dif );
  else if( strcmp( buf, "specular" ) == 0 )
    zRGBFScan( fp, &((zOpticalInfo *)instance)->spc );
  else if( strcmp( buf, "esr" ) == 0 )
    ((zOpticalInfo *)instance)->esr = zFDouble( fp );
  else if( strcmp( buf, "shininess" ) == 0 )
    ((zOpticalInfo *)instance)->sns = zFDouble( fp );
  else if( strcmp( buf, "alpha" ) == 0 )
    ((zOpticalInfo *)instance)->alpha = zFDouble( fp );
  else
    return false;
  return true;
}

/* scan information of the optical parameter set from a file. */
zOpticalInfo *zOpticalInfoFScan(FILE *fp, zOpticalInfo *oi)
{
  zOpticalInfoInit( oi );
  zFieldFScan( fp, _zOpticalInfoFScan, oi );
  if( zNamePtr( oi ) ) return oi;
  ZRUNERROR( ZEO_ERR_OPT_UNNAME );
  return NULL;
}

/* print information of the optical parameter set out to a file. */
void zOpticalInfoFPrint(FILE *fp, zOpticalInfo *oi)
{
  fprintf( fp, "name: %s\n", zName(oi) );
  fprintf( fp, "ambient: " );
  zRGBFPrint( fp, &oi->amb );
  fprintf( fp, "diffuse: " );
  zRGBFPrint( fp, &oi->dif );
  fprintf( fp, "specular: " );
  zRGBFPrint( fp, &oi->spc );
  fprintf( fp, "esr: %.10g\n", oi->esr );
  fprintf( fp, "shininess: %.10g\n", oi->sns );
  fprintf( fp, "alpha: %.10g\n", oi->alpha );
  fprintf( fp, "\n" );
}
