/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_render_color - rendering: color elements
 */

#include <zeo/zeo_render_color.h>
#include <math.h>

/* ********************************************************** */
/* RGB - expression of color with RGB intensity set.
 *//* ******************************************************* */

/* RGB sets for black and white */
#ifdef __cplusplus
const zRGB zRGB::zrgbblack = { 0.0, 0.0, 0.0 };
const zRGB zRGB::zrgbwhite = { 1.0, 1.0, 1.0 };
#else
const zRGB zrgbblack = { 0.0, 0.0, 0.0 };
const zRGB zrgbwhite = { 1.0, 1.0, 1.0 };
#endif /* __cplusplus */

/* set RGB parameters. */
zRGB *zRGBSet(zRGB *rgb, float red, float green, float blue)
{
  rgb->r = _zLimit( red,   0, 1 );
  rgb->g = _zLimit( green, 0, 1 );
  rgb->b = _zLimit( blue,  0, 1 );
  return rgb;
}

/* copy a set of RGB parameters. */
zRGB *zRGBCopy(const zRGB *src, zRGB *dest)
{
  _zRGBCopy( src, dest );
  return dest;
}

/* directly grayscalize a set of RGB parameters. */
zRGB *zRGBToGS(zRGB *rgb)
{
  zGSSet( rgb, zGS( rgb ) );
  return rgb;
}

/* multiply a set of RGB parameters by another. */
zRGB *zRGBMul(const zRGB *rgb1, const zRGB *rgb2, zRGB *rgb)
{
  return zRGBSet( rgb, rgb1->r*rgb2->r, rgb1->g*rgb2->g, rgb1->b*rgb2->b );
}

/* blend a pair of RGB parameters at a given ratio. */
zRGB *zRGBBlend(const zRGB *rgb1, const zRGB *rgb2, double ratio, zRGB *rgb)
{
  double rn;

  rn = 1 - ratio;
  rgb->r = ratio * rgb1->r + rn * rgb2->r;
  rgb->g = ratio * rgb1->g + rn * rgb2->g;
  rgb->b = ratio * rgb1->b + rn * rgb2->b;
  return rgb;
}

/* decode a string of sequential floating-point values to RGB. */
static zRGB *_zRGBRatio(zRGB *rgb, const char *ratio)
{
  float r, g, b;

  sscanf( ratio, "%f:%f:%f", &r, &g, &b );
  return zRGBSet( rgb, r, g, b );
}

/* decode a hexadecimal string to RGB. */
static zRGB *_zRGBHex(zRGB *rgb, const char *hex)
{
  int len, i;
  uint r, g, b, d;

  len = strlen( hex );
  if( len % 3 != 0 )
    ZRUNWARN( ZEO_ERR_INVALID_RGBSTR, hex, len );
  len /= 3;
  for( r=g=b=d=0, i=0; i<len; i++ ){
    r <<= 4; r |= zA2X_c( hex[i] );
    g <<= 4; g |= zA2X_c( hex[i+len] );
    b <<= 4; b |= zA2X_c( hex[i+len*2] );
    d <<= 4; d |= 0xf;
  }
  return zRGBSet( rgb, (float)r/d, (float)g/d, (float)b/d );
}

/* decode a string to RGB. */
zRGB *zRGBDecodeStr(zRGB *rgb, const char *str)
{
  return str[0] == '#' ? _zRGBHex( rgb, str+1 ) : _zRGBRatio( rgb, str );
}

/* find RGB by name. */
zRGB *zRGBByName(zRGB *rgb, const char *name)
{
  struct{
    const char *name;
    const float red;
    const float green;
    const float blue;
  } color_table[] = {
    { "black",   0, 0, 0 },
    { "red",     1, 0, 0 },
    { "green",   0, 1, 0 },
    { "blue",    0, 0, 1 },
    { "yellow",  1, 1, 0 },
    { "cyan",    0, 1, 1 },
    { "magenta", 1, 0, 1 },
    { "white",   1, 1, 1 },
    { NULL,      0, 0, 0 },
  };
  int i;

  if( name ){
    for( i=0; color_table[i].name; i++ )
      if( strcmp( color_table[i].name, name ) == 0 ){
        return zRGBSet( rgb, color_table[i].red, color_table[i].green, color_table[i].blue );
      }
  }
  zRGBCopy( ZRGBBLACK, rgb );
  return rgb;
}

/* find RGB by a string. */
zRGB *zRGBByStr(zRGB *rgb, const char *str)
{
  return str && ( str[0] == '#' || isdigit(str[0]) ) ? zRGBDecodeStr( rgb, str ) : zRGBByName( rgb, str );
}

/* read a set of RGB from a ZTK format processor. */
zRGB *zRGBFromZTK(zRGB *rgb, ZTK *ztk)
{
  if( ZTKVal(ztk)[0] == '#' )
    return zRGBDecodeStr( rgb, ZTKVal(ztk) );
  rgb->r = ZTKDouble(ztk);
  rgb->g = ZTKDouble(ztk);
  rgb->b = ZTKDouble(ztk);
  return rgb;
}

/* add RGB values to a ZTK format processor. */
ZTK *zRGBToZTK(const zRGB *rgb, ZTK *ztk)
{
  if( !ZTKAddDouble( ztk, rgb->r ) ) return NULL;
  if( !ZTKAddDouble( ztk, rgb->g ) ) return NULL;
  if( !ZTKAddDouble( ztk, rgb->b ) ) return NULL;
  return ztk;
}

/* scan a set of RGB parameters from a file. */
zRGB *zRGBFScan(FILE *fp, zRGB *rgb)
{
  double r, g, b;

  zFDouble( fp, &r );
  zFDouble( fp, &g );
  zFDouble( fp, &b );
  return zRGBSet( rgb, (float)r, (float)g, (float)b );
}

/* print a set of RGB parameters out to a file. */
void zRGBFPrint(FILE *fp, const zRGB *rgb)
{
  fprintf( fp, "%.7g, %.7g, %.7g\n", rgb->r, rgb->g, rgb->b );
}

/* ********************************************************** */
/* HSV - expression of color by hue, saturation and value.
 *//* ******************************************************* */

/* convert RGB to HSV. */
zHSV *zRGB2HSV(const zRGB *rgb, zHSV *hsv)
{
  float max, min, d, phase, phase0;

  /* find principal primary color */
  if( rgb->r > rgb->g ){
    if( rgb->r > rgb->b ){
      max = rgb->r;
      min = _zMin( rgb->g, rgb->b );
      phase = rgb->g - rgb->b;
      phase0 = 0;
    } else{
      max = rgb->b;
      min = rgb->g;
      phase = rgb->r - rgb->g;
      phase0 = 240;
    }
  } else{
    if( rgb->g > rgb->b ){
      max = rgb->g;
      min = _zMin( rgb->r, rgb->b );
      phase = rgb->b - rgb->r;
      phase0 = 120;
    } else{
      max = rgb->b;
      min = rgb->r;
      phase = rgb->r - rgb->g;
      phase0 = 240;
    }
  }
  d = max - min;
  /* conversion */
  hsv->hue =   d == 0 ? 0 : 60.0 * phase / d + phase0;
  if( hsv->hue < 0 ) hsv->hue += 360;
  hsv->sat = max == 0 ? 0 : d / max;
  hsv->val = max;
  return hsv;
}

/* convert HSV to RGB. */
zRGB *zHSV2RGB(const zHSV *hsv, zRGB *rgb)
{
  float h, f, p, q, t;
  int i;

  h = hsv->hue / 60;
  i = (int)floor( h ) % 6;
  f = h - i;
  p = hsv->val * ( 1 - hsv->sat );
  q = hsv->val * ( 1 - f * hsv->sat );
  t = hsv->val * ( 1 - (1-f) * hsv->sat );
  switch( i ){
  case 0: zRGBSet( rgb, hsv->val, t, p ); break;
  case 1: zRGBSet( rgb, q, hsv->val, p ); break;
  case 2: zRGBSet( rgb, p, hsv->val, t ); break;
  case 3: zRGBSet( rgb, p, q, hsv->val ); break;
  case 4: zRGBSet( rgb, t, p, hsv->val ); break;
  case 5: zRGBSet( rgb, hsv->val, p, q ); break;
  default: ;
  }
  return rgb;
}

/* print a set of HSV parameters out to a file. */
void zHSVFPrint(FILE *fp, const zHSV *hsv)
{
  fprintf( fp, "%.7g, %.7g, %.7g\n", hsv->hue, hsv->sat, hsv->val );
}
