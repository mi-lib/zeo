#include <zeo/zeo.h>
#include <float.h>

void assert_grayscale(void)
{
  zRGB rgb;
  float intensity;
  const int n = 100;
  int i;
  bool result = true;

  for( i=0; i<n; i++ ){
    zRGBSet( &rgb, zRandF(0,1), zRandF(0,1), zRandF(0,1) );
    intensity = zGS( &rgb );
    zRGBToGS( &rgb );
    if( !zIsTol(rgb.r-intensity,ZRGB_TOL) ||
        !zIsTol(rgb.g-intensity,ZRGB_TOL) ||
        !zIsTol(rgb.b-intensity,ZRGB_TOL) ) result = false;
  }
  zAssert( zRGBToGS, result );
}

void assert_decode_string(void)
{
  zRGB rgb;

  zRGBDecodeStr( &rgb, "#0f3df8" );
  zAssert( zRGBDecodeStr, zIsTol(rgb.r-(double)0x0f/0xff,ZRGB_TOL) &&
                          zIsTol(rgb.g-(double)0x3d/0xff,ZRGB_TOL) &&
                          zIsTol(rgb.b-(double)0xf8/0xff,ZRGB_TOL) );
}

/* difference of two hues on the circle */
double hue_diff(double h1, double h2)
{
  double d;

  return ( d = fabs( h1 - h2 ) ) > 180 ? 360 - d : d;
}

void assert_rgb_hsv(void)
{
  zRGB rgb, rgb_hsv;
  zHSV hsv, hsv_rgb;
  double d;
  const double tol = 10 * FLT_EPSILON;
  const int testnum = 1000;
  int i;
  bool result_rgb = true, result_hsv = true;

  for( i=0; i<testnum; i++ ){
    /* RGB -> HSV -> RGB */
    zRGBSet( &rgb, zRandF(0,1), zRandF(0,1), zRandF(0,1) );
    zRGB2HSV( &rgb, &hsv );
    zHSV2RGB( &hsv, &rgb_hsv );
    if( !zIsTol( rgb.r-rgb_hsv.r, tol ) ||
        !zIsTol( rgb.g-rgb_hsv.g, tol ) ||
        !zIsTol( rgb.b-rgb_hsv.b, tol ) ) result_rgb = false;
    /* HSV -> RGB -> HSV; hue and saturation are sensitive to errors of a dark or pale color. */
    hsv.hue = zRandF(0,360);
    hsv.sat = zRandF(0,1);
    hsv.val = zRandF(0,1);
    zHSV2RGB( &hsv, &rgb );
    zRGB2HSV( &rgb, &hsv_rgb );
    if( ( d = hsv.sat * hsv.val ) > 0 &&
        hue_diff( hsv.hue, hsv_rgb.hue ) > tol * ( 360 + 240 / d ) ) result_hsv = false;
    if( hsv.val > 0 && !zIsTol( hsv.sat-hsv_rgb.sat, tol * ( 1 + 1 / hsv.val ) ) ) result_hsv = false;
    if( !zIsTol( hsv.val-hsv_rgb.val, tol ) ) result_hsv = false;
  }
  zAssert( zRGB2HSV + zHSV2RGB (RGB-HSV-RGB), result_rgb );
  zAssert( zHSV2RGB + zRGB2HSV (HSV-RGB-HSV), result_hsv );
}

bool check_rgb(const zRGB *rgb, float r, float g, float b)
{
  return zIsTol( rgb->r-r, ZRGB_TOL ) && zIsTol( rgb->g-g, ZRGB_TOL ) && zIsTol( rgb->b-b, ZRGB_TOL );
}

void assert_hsv_boundary(void)
{
  zRGB rgb, rgb_hsv;
  zHSV hsv;

  hsv.sat = 0.5; hsv.val = 1;
  hsv.hue = 360;
  zAssert( zHSV2RGB (hue of 360), check_rgb( zHSV2RGB( &hsv, &rgb ), 1, 0.5, 0.5 ) );
  hsv.hue = -60;
  zAssert( zHSV2RGB (negative hue), check_rgb( zHSV2RGB( &hsv, &rgb ), 1, 0.5, 1 ) );
  hsv.hue = 420;
  zAssert( zHSV2RGB (hue larger than 360), check_rgb( zHSV2RGB( &hsv, &rgb ), 1, 1, 0.5 ) );
  /* a slightly negative hue, which might be rounded up to 360 */
  zRGBSet( &rgb, 1, 0.5, 0.5 + 0.5*FLT_EPSILON );
  zRGB2HSV( &rgb, &hsv );
  zHSV2RGB( &hsv, &rgb_hsv );
  zAssert( zRGB2HSV (hue close to 360), hsv.hue >= 0 && hsv.hue < 360 && check_rgb( &rgb_hsv, rgb.r, rgb.g, rgb.b ) );
}

int main(void)
{
  zRandInit();
  assert_grayscale();
  assert_decode_string();
  assert_rgb_hsv();
  assert_hsv_boundary();
  return EXIT_SUCCESS;
}
