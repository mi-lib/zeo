/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_render_color - rendering: color elements
 */

#include <zeo/zeo_render_color.h>
#include <math.h>

/* ********************************************************** */
/* CLASS: zRGB
 * RGB class - expression of color with RGB intensity set.
 * ********************************************************** */

static zRGB *_zRGBRatio(zRGB *rgb, char *ratio);
static zRGB *_zRGBHex(zRGB *rgb, char *hex);

/* RGB sets for black and white */
const zRGB zrgbblack = { 0.0, 0.0, 0.0 }, zrgbwhite = { 1.0, 1.0, 1.0 };

/* set RGB parameters. */
zRGB *zRGBSet(zRGB *rgb, float red, float green, float blue)
{
  rgb->r = _zLimit( red,   0, 1 );
  rgb->g = _zLimit( green, 0, 1 );
  rgb->b = _zLimit( blue,  0, 1 );
  return rgb;
}

/* multiply a set of RGB parameters by another. */
zRGB *zRGBMul(zRGB *rgb1, zRGB *rgb2, zRGB *rgb)
{
  return zRGBSet( rgb, rgb1->r*rgb2->r, rgb1->g*rgb2->g, rgb1->b*rgb2->b );
}

/* blend a pair of RGB parameters at a given ratio. */
zRGB *zRGBBlend(zRGB *rgb1, zRGB *rgb2, double ratio, zRGB *rgb)
{
  double rn;

  rn = 1 - ratio;
  rgb->r = ratio * rgb1->r + rn * rgb2->r;
  rgb->g = ratio * rgb1->g + rn * rgb2->g;
  rgb->b = ratio * rgb1->b + rn * rgb2->b;
  return rgb;
}

/* (static)
 * decode a string of sequential floating-point values to RGB. */
zRGB *_zRGBRatio(zRGB *rgb, char *ratio)
{
  float r, g, b;

  sscanf( ratio, "%f:%f:%f", &r, &g, &b );
  return zRGBSet( rgb, r, g, b );
}

/* (static)
 * decode a hexadecimal string to RGB. */
zRGB *_zRGBHex(zRGB *rgb, char *hex)
{
  int len, i;
  uint r, g, b, d;

  len = strlen( hex );
  if( len % 3 != 0 )
    ZRUNWARN( ZEO_ERR_RGB, hex, len );
  len /= 3;
  for( r=g=b=d=0, i=0; i<len; i++ ){
    r <<= 4; r |= atox_c( hex[i] );
    g <<= 4; g |= atox_c( hex[i+len] );
    b <<= 4; b |= atox_c( hex[i+len*2] );
    d <<= 4; d |= 0xf;
  }
  return zRGBSet( rgb, (float)r/d, (float)g/d, (float)b/d );
}

/* decode a string to RGB. */
zRGB *zRGBDec(zRGB *rgb, char *str)
{
  return str[0] == '#' ? _zRGBHex( rgb, str+1 ) : _zRGBRatio( rgb, str );
}

/* read a set of RGB from a ZTK format processor. */
zRGB *zRGBFromZTK(zRGB *rgb, ZTK *ztk)
{
  rgb->r = ZTKDouble(ztk);
  rgb->g = ZTKDouble(ztk);
  rgb->b = ZTKDouble(ztk);
  return rgb;
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
void zRGBFPrint(FILE *fp, zRGB *rgb)
{
  fprintf( fp, "%.7g, %.7g, %.7g\n", rgb->r, rgb->g, rgb->b );
}

/* ********************************************************** */
/* CLASS: zHSV
 * HSV class - expression of color by hue, saturation and value.
 * ********************************************************** */

/* convert RGB to HSV. */
zHSV *zRGB2HSV(zRGB *rgb, zHSV *hsv)
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
zRGB *zHSV2RGB(zHSV *hsv, zRGB *rgb)
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
void zHSVFPrint(FILE *fp, zHSV *hsv)
{
  fprintf( fp, "%.7g, %.7g, %.7g\n", hsv->hue, hsv->sat, hsv->val );
}
