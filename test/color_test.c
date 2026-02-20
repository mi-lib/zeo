#include <zeo/zeo.h>

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

void assert_rgb_hsv(void)
{
  zRGB rgb, rgb_hsv;
  zHSV hsv, hsv_rgb;

  zRGBSet( &rgb, zRandF(0,1), zRandF(0,1), zRandF(0,1) );
  zRGB2HSV( &rgb, &hsv );
  zHSV2RGB( &hsv, &rgb_hsv );
  zAssert( zRGB2HSV, zIsTol(rgb.r-rgb_hsv.r,ZRGB_TOL) &&
                     zIsTol(rgb.g-rgb_hsv.g,ZRGB_TOL) &&
                     zIsTol(rgb.b-rgb_hsv.b,ZRGB_TOL) );
  zRGB2HSV( &rgb_hsv, &hsv_rgb );
  zAssert( zHSV2RGB, zIsTol(hsv.hue-hsv_rgb.hue,ZRGB_TOL) &&
                     zIsTol(hsv.sat-hsv_rgb.sat,ZRGB_TOL) &&
                     zIsTol(hsv.val-hsv_rgb.val,ZRGB_TOL) );
}

int main(void)
{
  zRandInit();
  assert_grayscale();
  assert_decode_string();
  assert_rgb_hsv();
  return EXIT_SUCCESS;
}
