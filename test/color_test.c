#include <zeo/zeo.h>

#define RGB_TOL ( 1.0e-6 )

int main(void)
{
  zRGB rgb, rgb_hsv;
  zHSV hsv, hsv_rgb;

  zRGBDec( &rgb, "#0f3df8" );
  zAssert( zRGBDec, zIsTol(rgb.r-(double)0x0f/0xff,RGB_TOL) &&
                    zIsTol(rgb.g-(double)0x3d/0xff,RGB_TOL) &&
                    zIsTol(rgb.b-(double)0xf8/0xff,RGB_TOL) );
  zRGB2HSV( &rgb, &hsv );
  zHSV2RGB( &hsv, &rgb_hsv );
  zAssert( zRGB2HSV, zIsTol(rgb.r-rgb_hsv.r,RGB_TOL) &&
                     zIsTol(rgb.g-rgb_hsv.g,RGB_TOL) &&
                     zIsTol(rgb.b-rgb_hsv.b,RGB_TOL) );
  zRGB2HSV( &rgb_hsv, &hsv_rgb );
  zAssert( zHSV2RGB, zIsTol(hsv.hue-hsv_rgb.hue,RGB_TOL) &&
                     zIsTol(hsv.sat-hsv_rgb.sat,RGB_TOL) &&
                     zIsTol(hsv.val-hsv_rgb.val,RGB_TOL) );

  return EXIT_SUCCESS;
}
